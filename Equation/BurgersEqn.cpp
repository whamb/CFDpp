#include <cmath>
#include <numbers>

#include <BurgersEqn.hpp>

void BurgersEqn::initialiseSolution(const Mesh& mesh){
    const auto& cell               = mesh.getCells();
    const auto& cellCenter         = mesh.getCellCenter();
    const auto& interiorFace       = mesh.getInteriorFaces();
    const auto& boundaryFace       = mesh.getBoundaryFaces();
    const auto& faceArea           = mesh.getFaceArea();
    const auto& faceCenter         = mesh.getFaceCenter();

    // Initialise solution field
    for(const auto& c : cell){
        const CellID cId = c->getId();
        const Double x = cellCenter[cId];
        m_u[cId] = std::sin(std::numbers::pi * x); 
    }
    
    // Initialise interior face flux 
    for(const auto& f : interiorFace){
        const FaceID fId = f->getId();
        const Double area = faceArea[fId];
        auto [c0, c1] = f->getCellId();
        //TODO: geometric factor for irregular cells
        Double u0 = m_u[c0];
        Double u1 = m_u[c1];
        m_uf[fId] = 0.5 * (u0 + u1) * area;
    }

    // Initialise boundary face flux
    for(const auto& bF : boundaryFace){
        const FaceID bFId = bF->getId();
        const Double area = faceArea[bFId];
        auto c = bF->getCellId()[0];
       
        Double u = m_u[c];
        m_uf[bFId] = u * area;
    }
}

void BurgersEqn::buildBurgers(const Mesh& mesh, TripletSystem& tripletSystem){
    updateFaceFlux    (mesh);
    buildAdvectionTerm(mesh, tripletSystem);
    buildViscousTerm  (mesh, tripletSystem);
    buildTransientTerm(mesh, tripletSystem);
    updateBc          (mesh,tripletSystem);
}

void BurgersEqn::buildAdvectionTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells         = mesh.getCells();
    const auto& faces         = mesh.getFaces();
    const auto& normals       = mesh.getFaceNormal();
    
    for(const auto& cell : cells){
        const auto& cellId = cell->getId();
        const auto& faceIds = cell->getFaceIds();
        for(const auto& fId : faceIds){
            const auto& face = *(faces[fId]); //Look up for neighbour using global indexing
            const auto& neighbourId = cell->getNghbrCell(face);
            const auto& n = (cellId > neighbourId) ? normals[fId] 
                                                   : -normals[fId];
            if(neighbourId == -1){
                continue;                     //Check empty cell for boundary faces
            }
            
            const Double flux = m_uf[fId] * n;
            Double upwindCoeff   = 0.5 * posMax(flux);
            Double downwindCoeff = -0.5 * posMax(flux);
            
            tripletSystem.addToLHS(cellId, cellId, upwindCoeff);
            tripletSystem.addToLHS(cellId, neighbourId, downwindCoeff);
        }
    }
}

void BurgersEqn::buildViscousTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells  = mesh.getCells();
    const auto& faces  = mesh.getFaces();
    const auto& center = mesh.getCellCenter();
    const auto& area   = mesh.getFaceArea();

    for(const auto& cell : cells){
        const auto& cellId = cell->getId();
        const auto& faceIds = cell->getFaceIds();
        for(const auto& fId : faceIds){
            const auto& face = *(faces[fId]); //Look up for neighbour using global indexing
            const auto& neighbourId = cell->getNghbrCell(face);
            if(neighbourId == -1){
                continue;                     //Check empty cell for boundary faces
            }
            
            Double dx = std::abs(center[cellId] - center[neighbourId]);
            Double diffCoeff = m_nu * area[fId] / dx;
            
            tripletSystem.addToLHS(cellId, cellId, diffCoeff);
            tripletSystem.addToLHS(cellId, neighbourId, -diffCoeff);
        }
    }
}

void BurgersEqn::buildTransientTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells = mesh.getCells();
    const auto& vol   = mesh.getCellVolume();

    for(const auto& cell : cells){
        const CellID cellId = cell->getId();
        const Double transientCoeff = vol[cellId] / m_dt; 

        tripletSystem.addToLHS(cellId, cellId, transientCoeff);
        tripletSystem.addToRHS(cellId, transientCoeff * m_u[cellId]);  
    }
}

void BurgersEqn::updateBc(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& bndCells     = mesh.getBoundaryCells();
    const auto& cell0        = bndCells[0];
    const auto& cell1        = bndCells[1];
    const auto& cell0Id      = cell0->getId();
    const auto& cell1Id      = cell1->getId();
    const auto& cellCenter   = mesh.getCellCenter();
    const auto& bndFaces     = mesh.getBoundaryFaces();
    const auto& face0        = bndFaces[0];
    const auto& face1        = bndFaces[1];
    const auto& face0Id      = face0->getId();
    const auto& face1Id      = face1->getId();
    const auto& faceCenter   = mesh.getFaceCenter();
    const auto& area         = mesh.getFaceArea();
    const auto& normals      = mesh.getFaceNormal();

    // Advection part
    const auto& n0 = -normals[face0Id];
    const auto& n1 = -normals[face1Id];
    const Double advectionFlux0 = m_uf[face0Id] * n0;
    Double upwindCoeff0   = 0.5 * posMax(advectionFlux0);
    Double downwindCoeff0 = -0.5 * posMax(advectionFlux0);
    const Double advectionFlux1 = m_uf[face1Id] * n1;
    Double upwindCoeff1   = 0.5 * posMax(advectionFlux1);
    Double downwindCoeff1 = -0.5 * posMax(advectionFlux1);
    
    //Cell 0
    tripletSystem.addToLHS(cell0Id, cell0Id, upwindCoeff0);
    tripletSystem.addToLHS(cell0Id, cell1Id, downwindCoeff0);
    //Cell 1
    tripletSystem.addToLHS(cell1Id, cell1Id, upwindCoeff1);
    tripletSystem.addToLHS(cell1Id, cell0Id, downwindCoeff1);

    // Diffusion part
    const Double dx = std::abs(cellCenter[0] - faceCenter[0])
                    + std::abs(cellCenter[1] - faceCenter[1]);
    //TODO: check if area[face0] == area[face1]
    Double diffCoeff = m_nu * area[face0->getId()] / dx;

    // Cell0
    tripletSystem.addToLHS(cell0Id, cell0Id, diffCoeff);
    tripletSystem.addToLHS(cell0Id, cell1Id, -diffCoeff);
    // Cell1
    tripletSystem.addToLHS(cell1Id, cell1Id, diffCoeff);
    tripletSystem.addToLHS(cell1Id, cell0Id, -diffCoeff);
    
}

void BurgersEqn::updateFaceFlux(const Mesh& mesh){
    const auto& interiorFace       = mesh.getInteriorFaces();
    const auto& boundaryFace       = mesh.getBoundaryFaces();
    const auto& area               = mesh.getFaceArea();

    for(const auto& face : interiorFace){
        const FaceID faceId = face->getId();
        const CellID c0 = face->getCellId()[0];
        const CellID c1 = face->getCellId()[1];
        const Double fArea = area[faceId];
        const Double uf0 = m_u[c0] * fArea;
        const Double uf1 = m_u[c1] * fArea;
        m_uf[faceId] = 0.5 * (uf0 + uf1);
    }

    //Boundary faces 
    // TODO: duplicated code
    for(const auto& face : boundaryFace){
        const FaceID faceId = face->getId();
        const CellID c0 = face->getCellId()[0];
        const CellID c1 = face->getCellId()[1];
        const Double fArea = area[faceId];
        const Double uf0 = m_u[c0] * fArea;
        const Double uf1 = m_u[c1] * fArea;
        m_uf[faceId] = 0.5 * (uf0 + uf1);
    }
}
