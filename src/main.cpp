#include <chrono>
#include <iostream>

#include <BurgersEqn.hpp>
#include <EigenSolve.hpp>
#include <SolveBurgers.hpp>
#include <TripletSystem.hpp>
#include <CsrSystem.hpp>
#include <Mesh.hpp>
#ifdef USE_PETSC
#include <PetscCsrSolve.hpp>
#endif

// A mock class to expose protected members for test validation (optional)

int main(){
    Double tFinal = 100.0;
    Double dt = 0.01;
    Double nu = 0.1;

    auto begin = std::chrono::steady_clock::now();
    Mesh mesh(0.0, 1, 0.001); 
    auto meshing = std::chrono::steady_clock::now();

    BurgersEqn burgersEqn(mesh, dt, nu);
    SolveBurgers solveBurgers(mesh, tFinal, dt);
    #ifdef USE_PETSC
    PetscInitialize(NULL, NULL, NULL, NULL);
    #endif

    solveBurgers.cyclingStrategy(mesh, burgersEqn);
    auto solving = std::chrono::steady_clock::now();

    #ifdef USE_PETSC
    PetscFinalize();
    #endif

    std::cout << "Meshing Elapsed milliseconds: "
        << duration_cast<std::chrono::milliseconds>(meshing - begin).count()
        << "ms\n";

    std::cout << "Solving Elapsed milliseconds: "
        << duration_cast<std::chrono::milliseconds>(solving - meshing).count()
        << "ms\n";
    
    return 0;
}