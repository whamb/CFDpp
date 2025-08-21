#include <BurgersEqn.hpp>
#include <SolveBurgers.hpp>
#include <TripletSystem.hpp>
#include <CsrSystem.hpp>
#include <Mesh.hpp>
#include <PetscCsrSolve.hpp>

// A mock class to expose protected members for test validation (optional)

int main(){
    Double tFinal = 1.0;
    Double dt = 0.01;
    Double nu = 0.001;
    Mesh mesh(0.0, 3.0, 1.0); 

    //std::cout << "Print cells: " << "\n";
    //for(const auto cell : mesh.getCells()){
    //    std::cout << *cell << "\n";
    //}

    /*std::cout << "\n" <<"TESTING NEIGHBOUR FUNCTION";
    for(const auto& cell : mesh.getInteriorCells()){
        const double cellId = cell->getId();
        const auto& faceIds = cell->getFaceIds();
        std::cout << "\n" << "CellID = " << cellId << " ; ";
        for(const auto& faceId : faceIds){
            const auto& face = *(mesh.getFaces()[faceId]); 
            const auto& neighbourId = cell->getNghbrCell(face);
            std::cout << " FaceId = " << faceId << " ; ";
            std::cout << "NghbrCellId = " << neighbourId;
        }
    }*/

    BurgersEqn burgersEqn(mesh, dt, nu);
    SolveBurgers solveBurgers(mesh, tFinal, dt);
    solveBurgers.cyclingStrategy(mesh, burgersEqn);
    
    return 0;
}