#include <iostream>

#include <EigenSolve.hpp>
#ifdef USE_PETSC
#include <PetscCsrSolve.hpp>
#endif
#include <SolveBurgers.hpp>

void SolveBurgers::cyclingStrategy(const Mesh& mesh, BurgersEqn& burgersEqn){
    burgersEqn.initialiseSolution(mesh, m_tripletSystem);
    // Dump initial condition
    dumpSolution(mesh, burgersEqn.u());
    while(m_time < m_tFinal){
        advanceTime();
        m_tripletSystem.clear();
        burgersEqn.buildBurgers(mesh, m_tripletSystem);
        #ifdef USE_PETSC
        burgersEqn.u() = PetscCsrSolve::solveWithPETSc(mesh, CsrSystem(m_tripletSystem));
        #else
        burgersEqn.u() = EigenSolve::solveWithEigen(mesh, m_tripletSystem);
        #endif
        // Dump solution after each time step
        if(static_cast<int>(m_time / m_dt) % m_outputFrequency == 0)
            dumpSolution(mesh, burgersEqn.u());
    }
}

void SolveBurgers::advanceTime(){
    m_time += m_dt;
}

void SolveBurgers::dumpSolution(const Mesh& mesh, const ScalarField& u) {
    for (CellID i = 0; i < mesh.nCells(); ++i) {
        m_solutionFile << std::scientific << std::setprecision(4) <<
                          m_time << " " << 
                          mesh.cellCenter()[i] << " " << 
                          u[i] << 
                          "\n";
    }
    m_solutionFile.flush();
}