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
template<typename LHS, typename RHS, typename Solution>
class Equation 
{
public:

virtual Solution initialiseSolution() = 0;

virtual LHS assembleLHS() = 0;
virtual RHS assembleRHS() = 0;

virtual Solution solve(Double tolerance) = 0;

virtual ~Equation() = default;
};

#endif // EQUATION_HPP