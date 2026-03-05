#ifndef TRIDIAGONALSYSTEM_HPP
#define TRIDIAGONALSYSTEM_HPP

#include <span>
#include <vector>

#include <Types.hpp>

/**
 * @brief Represents the left-hand side (LHS) of a tridiagonal linear system.
 * 
 * Stores references to lower, upper, and diagonal coefficient arrays via std::span.
 * Provides access to matrix entries within the tridiagonal band.
 */
class TridiagonalLHS {
public:
    TridiagonalLHS(std::span<Double> lower,
                   std::span<Double> upper,
                   std::span<Double> diagonal,
                   CellID size);

    Double& operator()(CellID i, CellID j);
    TridiagonalLHS operator+(const TridiagonalLHS& lhs) const;

private:
    std::span<Double> m_lower;
    std::span<Double> m_diagonal;
    std::span<Double> m_upper;
    CellID m_size;
};

/**
 * @brief Represents the right-hand side (RHS) vector of a linear system.
 * 
 * Wraps a span of doubles to allow indexed access to the RHS entries.
 */
class TridiagonalRHS {
public:
    TridiagonalRHS(std::span<Double> rhs, CellID size){};

    Double& operator()(CellID i);
    TridiagonalRHS operator+(const TridiagonalRHS& rhs) const;

private:
    std::span<Double> m_rhs;
    CellID m_size;
};

/**
 * @brief A complete tridiagonal linear system (A·x = b).
 * 
 * Owns storage for the lower, diagonal, upper, and RHS vectors.
 * Provides methods for initialization and solving via the Thomas algorithm.
 * Interfaces with polymorphic LinearSystem for general solver infrastructure.
 */
class TridiagonalSystem {
public:
    TridiagonalSystem(CellID size) : 
        m_size(size),
        m_lower(size - 1, 0.0),
        m_upper(size - 1, 0.0),
        m_diagonal(size, 0.0),
        m_rhs(size, 0.0)
    {}

    void initialiseTridiagonalSystem();  // Optional zeroing or custom init
    std::vector<Double> thomasSolve() const;

private:
    CellID m_size;
    std::vector<Double> m_lower;
    std::vector<Double> m_diagonal;
    std::vector<Double> m_upper;
    std::vector<Double> m_rhs;
};

#endif // TRIDIAGONALSYSTEM_HPP