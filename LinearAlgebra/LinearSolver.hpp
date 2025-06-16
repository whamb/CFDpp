#ifndef LINEARSOLVER_HPP
#define LINEARSOLVER_HPP

#include <vector>
#include "LinearSystem.hpp"

/**
 * @brief Abstract interface for linear system solvers.
 * 
 * Defines the contract for solving systems of the form A·x = b,
 * where A and b are provided as abstract LHS and RHS interfaces.
 * Derived classes may implement specific algorithms (e.g., Thomas, CG, LU).
 */
class LinearSolver {
public:
    virtual std::vector<Double> solve(LHS& A, RHS& b) = 0;
};

#endif // LINEARSOLVER_HPP