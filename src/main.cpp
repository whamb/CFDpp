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
    Mesh mesh(0.0, 10.0, 1.0); 
    BurgersEqn(mesh, dt, nu);
    SolveBurgers(mesh, tFinal, dt);
    return 0;
}