#ifndef SOLVEBURGERS_HPP
#define SOLVEBURGERS_HPP

#include <fstream>
#include <iomanip>

#include <BurgersEqn.hpp>
#include <Mesh.hpp>
#include <TripletSystem.hpp>
#include <Types.hpp>
#include <EigenSolve.hpp>
#ifdef USE_PETSC
#include <PetscCsrSolve.hpp>
#endif

class SolveBurgers
{
public:
    SolveBurgers(const Mesh& mesh, const Double tFinal, const Double dt, const int outputFrequency) : 
        m_tripletSystem(TripletSystem(mesh)),
        m_tFinal(tFinal),
        m_dt(dt),
        m_outputFrequency(outputFrequency),
        m_solutionFile("burgers_solution.dat")
    {
        if(dt < m_time || tFinal < dt)
            throw std::runtime_error("Time step or final time are incorrect");
        
        // Write header
        m_solutionFile << "# time x u\n";
    }

    void cyclingStrategy(const Mesh& mesh, BurgersEqn& burgersEqn);
    void advanceTime();
    void dumpSolution(const Mesh& mesh, const ScalarField& u);
    //Double computeResiduals(const TripletSystem& tripletSystem);
    //bool checkConvergence();

private:
    bool m_isConverged = false;
    Double m_tFinal;
    Double m_dt;
    Double m_time = 0.0;
    int m_outputFrequency;
    TripletSystem m_tripletSystem;
    std::ofstream m_solutionFile;
};

#endif // SOLVEBURGERS