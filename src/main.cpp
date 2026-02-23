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
    Mesh mesh(0.0, 1, 0.01); 

    BurgersEqn burgersEqn(mesh, dt, nu);
    SolveBurgers solveBurgers(mesh, tFinal, dt);
    PetscInitialize(NULL, NULL, NULL, NULL);
    solveBurgers.cyclingStrategy(mesh, burgersEqn);
    PetscFinalize();
    
    return 0;
}