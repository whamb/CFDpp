#ifndef EQUATION_HPP
#define EQUATION_HPP

#include "ScalarField.hpp"

/**
 * @brief Abstract interface for discretized equation solvers.
 * 
 * Defines the minimal contract for all PDE solvers: the ability to assemble
 * system components and solve for a result. Concrete equation classes (e.g., Burgers)
 * must implement this interface.
 */
class Equation 
{
public:
    virtual void assemble() = 0;
    virtual void solve(ScalarField& result) = 0;
};

#endif // EQUATION_HPP