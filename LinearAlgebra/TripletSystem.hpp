#ifndef TRIPLETSYSTEM_HPP
#define TRIPLETSYSTEM_HPP

#include <span>
#include <vector>

#include <Mesh.hpp>
#include <Types.hpp>

/**
 * @brief Represents a sparse linear system in triplet (COO) format.
 * 
 * A `TripletSystem` stores a sparse matrix as three separate arrays:
 * - `m_value`: Non-zero matrix values.
 * - `m_row`: Corresponding row indices.
 * - `m_column`: Corresponding column indices.
 * 
 * It also stores the right-hand side (RHS) vector `m_rhs`, which is sized according to the mesh.
 * This format is commonly used for incremental assembly of linear systems before conversion
 * to more efficient formats such as Compressed Sparse Row (CSR).
 * 
 * The class provides interfaces for:
 * - Adding matrix entries (`add`)
 * - Accessing internal triplet data (`getValues`, `getRows`, `getColumns`, `getRHS`)
 * - Modifying RHS entries (`addToRHS`, `setRHS`)
 * 
 * The design assumes cell-based indexing derived from a 1D finite volume mesh.
 */
class TripletSystem
{
public:
TripletSystem(Mesh& mesh) { m_rhs.resize(mesh.getNCells()); }

void addToLHS(Double value, CellID row, CellID column) {
    m_value.push_back(value);
    m_row.push_back(row);
    m_column.push_back(column);
}

void addToRHS(CellID row, Double value) {
    m_rhs[row] += value;
}

void setRHS(CellID row, Double value) {
    m_rhs[row] = value;
}

CellID rhsSize(){
    return m_rhs.size();
}

CellID lhsSize(){
    return m_value.size();
}

std::span<const Double> getValues() const {
    return m_value;
}

std::span<const CellID> getRows() const {
    return m_row;
}

std::span<const CellID> getColumns() const {
    return m_column;
}

std::span<const Double> getRHS() const {
    return m_rhs;
}

private:
std::vector<Double> m_value;
std::vector<CellID> m_row;
std::vector<CellID> m_column;
std::vector<Double> m_rhs;
};

#endif // TRIPLETSYSTEM_HPP