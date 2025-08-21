#include "PetscCsrSolve.hpp"
#include "SolveBurgers.hpp"

void SolveBurgers::cyclingStrategy(const Mesh& mesh, BurgersEqn& burgersEqn){
    burgersEqn.initialiseSolution(mesh);
    while(m_time < m_tFinal){
        std::cout << "Time = " << m_time << "\n";
        advanceTime();

        m_tripletSystem.clear();
        burgersEqn.buildBurgers(mesh, m_tripletSystem);
        burgersEqn.getU() = PetscCsrSolve::solveWithPETSc(mesh, CsrSystem(m_tripletSystem));
    }
}

void SolveBurgers::advanceTime(){
    m_time += m_dt;
}