#ifndef LINEARSYSTEM_HPP
#define LINEARSYSTEM_HPP

#include <span>
#include <vector>
#include <Types.hpp>

#include <LinSolverType.hpp>

/**
 * @brief Abstract interface for the left-hand side (matrix) of a linear system.
 * 
 * Supports access to matrix entries via 2D indexing. Implementations may represent
 * dense, tridiagonal, or sparse matrices.
 */
class LHS {
public:
virtual ~LHS() = default;
};

/**
 * @brief Abstract interface for the right-hand side vector of a linear system.
 * 
 * Supports access to RHS entries via 1D indexing. Implementations wrap various storage strategies.
 */
class RHS {
public:
virtual ~RHS() = default;
};

/**
 * @brief Encapsulates a complete linear system of the form A·x = b.
 * 
 * Holds polymorphic interfaces to a matrix (LHS) and a vector (RHS), allowing
 * flexible integration with different storage formats and solvers.
 */
class LinearSystem {
public:
virtual ~LinearSystem() = default;

protected:
LinearSystem(std::unique_ptr<LHS> lhs, std::unique_ptr<RHS> rhs)
    : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

std::unique_ptr<LHS> m_lhs;
std::unique_ptr<RHS> m_rhs;
};

#endif // LINEARSYSTEM_HPP