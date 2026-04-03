#ifndef LINEARSYSTEM_HPP
#define LINEARSYSTEM_HPP

#include <span>

#include <Types.hpp>

/**
 * @brief Represents the left-hand side (LHS) matrix of a CSR linear system.
 * 
 * Encapsulates a constant view (read-only) of the compressed sparse row (CSR) matrix data:
 * - `m_value` contains non-zero values.
 * - `m_rowIdx` holds the column indices corresponding to each value.
 * - `m_compressedRow` encodes the start of each row in `m_value`.
 * 
 * This struct is a lightweight wrapper that enables matrix-vector operations and solver access.
 */

struct Lhs {
    std::span<const Double> m_value;
    std::span<const CellID> m_row;
    std::span<const CellID> m_column;

    Lhs(std::span<const Double> value,
        std::span<const CellID> row,
        std::span<const CellID> column) : 
        m_value(value),
        m_row(row),
        m_column(column)
    {};
};

/**
 * @brief Represents the right-hand side (RHS) vector of a CSR linear system.
 * 
 * This struct wraps a constant view of the right-hand side vector used in solving
 * linear systems of the form Ax = b.
 */

struct Rhs {
    std::span<const Double> m_rhs;

    Rhs(std::span<const Double> rhs) : m_rhs(rhs)
    {};
};

#endif // LINEARSYSTEM_HPP