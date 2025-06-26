#ifndef CGSOLVER_HPP
#define CGSOLVER_HPP

#include "LinearSolver.hpp"

/**
 * @brief Conjugate Gradient (CG) iterative solver for symmetric positive-definite linear systems.
 * 
 * Solves systems of the form A·x = b where A is assumed to be symmetric and positive-definite.
 * This method is suitable for large sparse systems and operates efficiently in conjunction
 * with matrix representations like CSR.
 * 
 * Requirements:
 * - LHS (A) must be symmetric and positive-definite.
 * - The matrix and RHS are provided through the abstract LHS and RHS interfaces.
 * 
 * This solver does not perform preconditioning by default.
 */
class CGSolver : public LinearSolver
{
public:
    /**
     * @brief Solves the linear system A·x = b using the Conjugate Gradient algorithm.
     * 
     * @param A The left-hand side matrix (must be SPD).
     * @param b The right-hand side vector.
     * @return std::vector<Double> The solution vector x.
     */
    std::vector<Double> solve(LHS& A, RHS& b) override;
};

#endif // CGSOLVER_HPP