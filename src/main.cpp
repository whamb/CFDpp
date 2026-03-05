#include <chrono>
#include <iostream>


#include <BurgersEqn.hpp>
#include <SolveBurgers.hpp>
#include <TripletSystem.hpp>
#include <CsrSystem.hpp>
#include <Mesh.hpp>
#include <PetscCsrSolve.hpp>

// A mock class to expose protected members for test validation (optional)

int main(){
    Double tFinal = 100.0;
    Double dt = 0.05;
    Double nu = 0.1;
    Mesh mesh(0.0, 1, 0.01); 

    BurgersEqn burgersEqn(mesh, dt, nu);
    SolveBurgers solveBurgers(mesh, tFinal, dt);
    PetscInitialize(NULL, NULL, NULL, NULL);

    auto begin = std::chrono::steady_clock::now();
    solveBurgers.cyclingStrategy(mesh, burgersEqn);
    auto end = std::chrono::steady_clock::now();

    PetscFinalize();

     std::cout << "Elapsed milliseconds: "
        << duration_cast<std::chrono::milliseconds>(end - begin).count()
        << "ms\n";
    
    return 0;
}