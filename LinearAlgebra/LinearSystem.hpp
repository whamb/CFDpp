#ifndef LINEARSYSTEM_HPP
#define LINEARSYSTEM_HPP

#include <span>
#include <vector>
#include <Types.hpp>

/**
 * @brief Abstract interface for the left-hand side (matrix) of a linear system.
 * 
 * Supports access to matrix entries via 2D indexing. Implementations may represent
 * dense, tridiagonal, or sparse matrices.
 */
class LHS {
public:
    virtual ~LHS() = default;
    virtual Double& operator()(CellID i, CellID j) = 0;
};

/**
 * @brief Abstract interface for the right-hand side vector of a linear system.
 * 
 * Supports access to RHS entries via 1D indexing. Implementations wrap various storage strategies.
 */
class RHS {
public:
    virtual ~RHS() = default;
    virtual Double& operator()(CellID i) = 0;
};

/**
 * @brief Encapsulates a complete linear system of the form A·x = b.
 * 
 * Holds polymorphic interfaces to a matrix (LHS) and a vector (RHS), allowing
 * flexible integration with different storage formats and solvers.
 */
class LinearSystem {
public:
    LinearSystem(std::unique_ptr<LHS> A, std::unique_ptr<RHS> b)
        : m_A(std::move(A)), m_b(std::move(b)) {}

    Double& lhs(CellID i, CellID j) { return (*m_A)(i, j); }
    Double& rhs(CellID i) { return (*m_b)(i); }

private:
    std::unique_ptr<LHS> m_A;
    std::unique_ptr<RHS> m_b;
};

#endif // LINEARSYSTEM_HPP