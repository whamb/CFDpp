#ifndef THOMASSOLVER_HPP
#define THOMASSOLVER_HPP

#include "LinearSolver.hpp"

/**
 * @brief Solver for tridiagonal linear systems using the Thomas algorithm.
 * 
 * Implements a direct method for solving A·x = b where A is a tridiagonal matrix.
 * Assumes that the LHS provided supports tridiagonal access (i.e., A(i, j) is defined
 * only for j = i-1, i, or i+1).
 */
class ThomasSolver : public LinearSolver
{
public:
    std::vector<Double> solve(LHS& A, RHS& b) override;
};

#endif // THOMASSOLVER_HPP