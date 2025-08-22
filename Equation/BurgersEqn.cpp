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
    //buildAdvectionTerm(mesh, tripletSystem);
    buildViscousTerm  (mesh, tripletSystem);
    buildTransientTerm(mesh, tripletSystem);
    updateBc          (mesh,tripletSystem);
}

void BurgersEqn::buildAdvectionTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells         = mesh.getInteriorCells();
    const auto& faces         = mesh.getInteriorFaces();
    const auto& normals       = mesh.getFaceNormal();
    
    for(const auto& cell : cells){
        const auto& cellId = cell->getId();
        const auto& faceIds = cell->getFaceIds();
        for(const auto& fId : faceIds){
            const auto& face = *(mesh.getFaces()[fId]); //Look up for neighbour using global indexing
            const auto& neighbourId = cell->getNghbrCell(face);
            const auto& n = (cellId > neighbourId) ? normals[fId] 
                                                   : -normals[fId];
            
            const double flux = m_uf[fId] * n;
            double upwindCoeff   = 0.5 * posMax(flux);
            double downwindCoeff = -0.5 * posMax(flux);
            
            tripletSystem.addToLHS(cellId, cellId, upwindCoeff);
            tripletSystem.addToLHS(cellId, neighbourId, downwindCoeff);
        }
    }
}

void BurgersEqn::buildViscousTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells  = mesh.getInteriorCells();
    const auto& faces  = mesh.getInteriorFaces();
    const auto& center = mesh.getFaceCenter();
    const auto& area   = mesh.getFaceArea();

    for(const auto& cell : cells){
        const auto& cellId = cell->getId();
        const auto& faceIds = cell->getFaceIds();
        for(const auto& fId : faceIds){
            const auto& face = *(mesh.getFaces()[fId]); //Look up for neighbour using global indexing
            const auto& neighbourId = cell->getNghbrCell(face);
            
            Double dx = std::abs(center[cellId] - center[neighbourId]);
            Double diffCoeff = m_nu * area[fId] / dx;
            
            tripletSystem.addToLHS(cellId, cellId, -diffCoeff);
            tripletSystem.addToLHS(cellId, neighbourId, diffCoeff);
        }
    }
}

void BurgersEqn::buildTransientTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells = mesh.getCells();
    const auto& vol  = mesh.getCellVolume();

    for(const auto& cell : cells){
        const CellID cellId = cell->getId();
        const Double transientCoeff = vol[cellId] / m_dt; 

        tripletSystem.addToLHS(cellId, cellId, transientCoeff);
        tripletSystem.addToRHS(cellId, transientCoeff * m_u[cellId]);  
    }
}

void BurgersEqn::updateBc(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells         = mesh.getBoundaryCells();
    const auto& leftBndCell   = cells[0]->getId();
    const auto& rightBndCell  = cells[1]->getId();

    // Left boundary
    const Double penalty = 1.0;
    tripletSystem.addToLHS(leftBndCell, leftBndCell, penalty);
    tripletSystem.addToLHS(leftBndCell, rightBndCell, -penalty);
}

void BurgersEqn::updateFaceFlux(const Mesh& mesh){
    const auto& interiorFace       = mesh.getInteriorFaces();

    for(const auto& face : interiorFace){
        const FaceID faceId = face->getId();
        const CellID c0 = face->getCellId()[0];
        const CellID c1 = face->getCellId()[1];
        m_uf[faceId] = 0.5 * (m_uf[c0] + m_uf[c1]);
    }
}
