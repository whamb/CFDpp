#include <BurgersEqn.hpp>
#include <SolveBurgers.hpp>
#include <TripletSystem.hpp>
#include <CsrSystem.hpp>
#include <Mesh.hpp>
#include <PetscCsrSolve.hpp>

// A mock class to expose protected members for test validation (optional)

int main(){
    Double tFinal = 5.0;
    Double dt = 0.05;
    Double nu = 0.1;
    //Mesh(const Double lowerBound, const Double upperBound, const Double dx)
    Mesh mesh(0.0, 1, 0.01); 
    const auto& cells = mesh.getCells();
    const auto& faces = mesh.getFaces();

    BurgersEqn burgersEqn(mesh, dt, nu);
    SolveBurgers solveBurgers(mesh, tFinal, dt);
    PetscInitialize(NULL, NULL, NULL, NULL);
    solveBurgers.cyclingStrategy(mesh, burgersEqn);
    PetscFinalize();
    
    return 0;
}