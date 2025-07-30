#include <cmath>
#include <numbers>

#include <Burgers.hpp>

void Burgers::initialiseSolution(Mesh& mesh){
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

void Burgers::buildBurgers(Mesh& mesh, TripletSystem& tripletSystem){
    buildAdvectionTerm(mesh, tripletSystem);
    buildViscousTerm(mesh, tripletSystem);
    buildTransientTerm(mesh, tripletSystem);
}

void Burgers::buildAdvectionTerm(Mesh& mesh, TripletSystem& tripletSystem){
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

void Burgers::buildViscousTerm(Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells = mesh.getInteriorCells();
    const auto& faces = mesh.getInteriorFaces();
    
    for(const auto& c : cells){

    }
}

void Burgers::updateBc(Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells         = mesh.getBoundaryCells();
    const auto& leftBndCell   = cells[0]->getId();
    const auto& rightBndCell  = cells[1]->getId();

    // Left boundary
    const Double penalty = 1.0;
    tripletSystem.addToLHS(leftBndCell, leftBndCell, penalty);
    tripletSystem.addToLHS(leftBndCell, rightBndCell, -penalty);
}

Double Burgers::advanceTime(){
    return m_time + m_dt;
}