#include <cmath>
#include <numbers>

#include <BurgersEqn.hpp>

void Burgers::initialiseSolution(const Mesh& mesh){
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

void Burgers::buildBurgers(const Mesh& mesh, TripletSystem& tripletSystem){
    updateFaceFlux    (mesh);
    buildAdvectionTerm(mesh, tripletSystem);
    buildViscousTerm  (mesh, tripletSystem);
    buildTransientTerm(mesh, tripletSystem);
    updateBc          (mesh,tripletSystem);
}

void Burgers::buildAdvectionTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells         = mesh.getInteriorCells();
    const auto& faces         = mesh.getInteriorFaces();
    const auto& normals       = mesh.getFaceNormal();
    
    for(const auto& cell : cells){
        const double cellId = cell->getId();
        const auto& faceIds = cell->getFaceIds();
        for(const auto& faceId : faceIds){
            const auto& face = *faces[faceId]; 
            const auto& neighbourId = cell->getNghbrCell(face);
            const auto& n = (cellId > neighbourId) ? normals[faceId] 
                                                   : -normals[faceId];
            
            const double flux = m_uf[faceId] * n;
            double upwindCoeff   = 0.5 * posMax(flux);
            double downwindCoeff = -0.5 * posMax(flux);
            
            tripletSystem.addToLHS(cellId, cellId, upwindCoeff);
            tripletSystem.addToLHS(cellId, neighbourId, downwindCoeff);
        }
    }
}

void Burgers::buildViscousTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells = mesh.getInteriorCells();
    const auto& faces = mesh.getInteriorFaces();
}

void Burgers::buildTransientTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells = mesh.getInteriorCells();
    const auto& vol  = mesh.getCellVolume();

    for(const auto& cell : cells){
        const CellID cellId = cell->getId();
        const Double transientCoeff = vol[cellId] / m_dt; 

        tripletSystem.addToLHS(cellId, cellId, transientCoeff);
        tripletSystem.addToRHS(cellId, transientCoeff * m_u[cellId]);  
    }
}

void Burgers::updateBc(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells         = mesh.getBoundaryCells();
    const auto& leftBndCell   = cells[0]->getId();
    const auto& rightBndCell  = cells[1]->getId();

    // Left boundary
    const Double penalty = 1.0;
    tripletSystem.addToLHS(leftBndCell, leftBndCell, penalty);
    tripletSystem.addToLHS(leftBndCell, rightBndCell, -penalty);
}

void Burgers::updateFaceFlux(const Mesh& mesh){
    const auto& interiorFace       = mesh.getInteriorFaces();

    for(const auto& face : interiorFace){
        const FaceID faceId = face->getId();
        const CellID c0 = face->getCellId()[0];
        const CellID c1 = face->getCellId()[1];
        m_uf[faceId] = 0.5 * (m_uf[c0] + m_uf[c1]);
    }
}

Double Burgers::advanceTime(){
    return m_time + m_dt;
}