#ifndef SOLVEBURGERS_HPP
#define SOLVEBURGERS_HPP

#include "BurgersEqn.hpp"
#include "Mesh.hpp"
#include "TripletSystem.hpp"
#include "Types.hpp"

class SolveBurgers
{
public:
SolveBurgers(const Mesh& mesh, const Double tFinal, const Double dt) : 
    m_tripletSystem(TripletSystem(mesh)),
    m_tFinal(tFinal),
    m_dt(dt)
{
    if(dt < m_time || tFinal < dt)
        throw std::runtime_error("Time step or final time are incorrect");
}

void cyclingStrategy(const Mesh& mesh, BurgersEqn& burgersEqn);
void advanceTime();
//Double computeResiduals(const TripletSystem& tripletSystem);
//bool checkConvergence();

private:
bool m_isConverged = false;
Double m_tFinal;
Double m_dt;
Double m_time = 0.0;
TripletSystem m_tripletSystem;

};

#endif // SOLVEBURGERS