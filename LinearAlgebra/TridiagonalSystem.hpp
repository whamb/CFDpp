#ifndef TRIDIAGONALSYSTEM_HPP
#define TRIDIAGONALSYSTEM_HPP

#include <vector>
#include <LinearSystem.hpp>
#include <Types.hpp>

/**
 * @brief Represents the left-hand side (LHS) of a tridiagonal linear system.
 * 
 * Stores references to lower, upper, and diagonal coefficient arrays via std::span.
 * Provides access to matrix entries within the tridiagonal band.
 */
class TridiagonalLHS : public LHS {
public:
    TridiagonalLHS(std::span<Double> lower,
                   std::span<Double> upper,
                   std::span<Double> diagonal,
                   CellID size);

    Double& operator()(CellID i, CellID j) override;

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
class TridiagonalRHS : public RHS {
public:
    TridiagonalRHS(std::span<Double> b, CellID size);

    Double& operator()(CellID i) override;

private:
    std::span<Double> m_b;
    CellID m_size;
};

/**
 * @brief A complete tridiagonal linear system (A·x = b).
 * 
 * Owns storage for the lower, diagonal, upper, and RHS vectors.
 * Provides methods for initialization and solving via the Thomas algorithm.
 * Interfaces with polymorphic LinearSystem for general solver infrastructure.
 */
class TridiagonalSystem : public LinearSystem {
public:
    TridiagonalSystem(CellID size);

    void initialiseTridiagonalSystem(CellID size);
    std::vector<Double> thomasSolve() const;

private:
    CellID m_size;
    std::vector<Double> m_lower;
    std::vector<Double> m_diagonal;
    std::vector<Double> m_upper;
    std::vector<Double> m_b;
};

#endif // TRIDIAGONALSYSTEM_HPP