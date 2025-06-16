#include <TridiagonalSystem.hpp>

Double& TridiagonalLHS::operator()(CellID i, CellID j){
    if (i < 0 || j < 0 || i > m_size-1 || j > m_size-1) {
        throw std::out_of_range("Index out of bounds in TridiagonalSystem(i,j)");
    }
    if(i==j) return m_diagonal[i];
    else if(i==j-1) return m_upper[i];
    else if(i==j+1) return m_lower[i];
    else throw std::invalid_argument("Only main, upper, and lower diagonal elements are accessible");
}

Double& TridiagonalRHS::operator()(const CellID i){
    if (i < 0 || i > m_size-1) {
        throw std::out_of_range("Index out of bounds in TridiagonalSystem(i)");
    }
    return m_b[i];
}


