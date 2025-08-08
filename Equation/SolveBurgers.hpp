#ifndef SOLVEBURGERS_HPP
#define SOLVEBURGERS_HPP

#include "TripletSystem.hpp"
#include "Types.hpp"

class SolveBurgers
{
public:
Double advanceTime();
Double computeResiduals(const TripletSystem& tripletSystem);
bool checkConvergence();

private:
bool m_isConverged = false;
Double m_tFinal;
Double m_dt;
Double m_time = 0.0;

};

#endif // SOLVEBURGERS