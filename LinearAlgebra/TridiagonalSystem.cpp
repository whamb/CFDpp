#include <TridiagonalSystem.hpp>

/**
 * @brief Access an element of the tridiagonal matrix A(i, j).
 *
 * Only the main diagonal, upper diagonal (i == j - 1), and lower diagonal (i == j + 1) 
 * are valid accesses. Any other (i, j) combination is invalid and will throw.
 *
 * @param i Row index
 * @param j Column index
 * @return Reference to the corresponding matrix element
 * @throws std::out_of_range if i or j are out of bounds
 * @throws std::invalid_argument if (i, j) is not on a tridiagonal band
 */
Double& TridiagonalLHS::operator()(CellID i, CellID j) {
    if (i < 0 || j < 0 || i >= m_size || j >= m_size) {
        throw std::out_of_range("Index out of bounds in TridiagonalSystem(i,j): i=" 
            + std::to_string(i) + ", j=" + std::to_string(j));
    }

    if (i == j) return m_diagonal[i];
    if (i == j - 1) return m_upper[i];
    if (i == j + 1) return m_lower[i];

    throw std::invalid_argument("Only main, upper, and lower diagonal elements are accessible: i=" 
        + std::to_string(i) + ", j=" + std::to_string(j));
}

/**
 * @brief Access an element of the right-hand-side vector b(i).
 *
 * @param i Index of the RHS entry
 * @return Reference to b[i]
 * @throws std::out_of_range if i is out of bounds
 */
Double& TridiagonalRHS::operator()(const CellID i) {
    if (i < 0 || i >= m_size) {
        throw std::out_of_range("Index out of bounds in TridiagonalSystem(i): i=" + std::to_string(i));
    }
    return m_b[i];
}