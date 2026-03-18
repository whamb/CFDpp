#include <stdexcept>
#include <string>

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

TridiagonalLHS TridiagonalLHS::operator+(const TridiagonalLHS& lhs) const {
    if (m_size != lhs.m_size) {
        throw std::invalid_argument("TridiagonalLHS::operator+: size mismatch");
    }
    TridiagonalLHS resultLHS(m_upper,
                             m_lower,
                             m_diagonal,
                             m_size);

    for(int i=0; i<m_size-1; ++i){
        resultLHS.m_upper[i]    += lhs.m_upper[i];     
        resultLHS.m_lower[i]    += lhs.m_lower[i];
        resultLHS.m_diagonal[i] += lhs.m_diagonal[i];
    }
    resultLHS.m_diagonal[m_size-1] += lhs.m_diagonal[m_size-1];
    return resultLHS;
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
    return m_rhs[i];
}

TridiagonalRHS TridiagonalRHS::operator+(const TridiagonalRHS& rhs) const {
    if (m_size != rhs.m_size) {
        throw std::invalid_argument("TridiagonalRHS::operator+: size mismatch");
    }
    TridiagonalRHS resultRHS(m_rhs,
                             m_size);

    for(int i=0; i<m_size; ++i){
        resultRHS.m_rhs[i] += rhs.m_rhs[i];
    }
    return resultRHS;
}  
