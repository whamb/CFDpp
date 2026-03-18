#include <iostream>

#include <EigenSolve.hpp>
#ifdef USE_PETSC
#include <PetscCsrSolve.hpp>
#endif
#include <SolveBurgers.hpp>

void SolveBurgers::cyclingStrategy(const Mesh& mesh, BurgersEqn& burgersEqn){
    burgersEqn.initialiseSolution(mesh);
    advanceTime();
    while(m_time < m_tFinal){
        m_tripletSystem.clear();
        burgersEqn.buildBurgers(mesh, m_tripletSystem);
        #ifdef USE_PETSC
        burgersEqn.u() = PetscCsrSolve::solveWithPETSc(mesh, CsrSystem(m_tripletSystem));
        #else
        burgersEqn.u() = EigenSolve::solveWithEigen(mesh, m_tripletSystem);
        #endif
    }
}

void SolveBurgers::advanceTime(){
    m_time += m_dt;
}