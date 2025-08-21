#ifndef TRIPLETSYSTEM_HPP
#define TRIPLETSYSTEM_HPP

#include <span>
#include <vector>
#include <unordered_map>

#include <Mesh.hpp>
#include <Types.hpp>

struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const noexcept {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);  // hash combine
    }
};

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
TripletSystem(const Mesh& mesh);

void addToLHS(const CellID row, const CellID column, const Double value);
void addToRHS(const CellID row, const Double value) {m_rhs[row] += value;}
void setRHS  (const CellID row, const Double value) {m_rhs[row] = value;}

const CellID rhsSize(){return m_rhs.size();}
const CellID lhsSize(){return m_value.size();}

const std::span<const Double> getValues() const {return m_value;}
const std::span<const CellID> getRows() const {return m_row;}
const std::span<const CellID> getColumns() const {return m_column;}
const std::span<const Double> getRHS() const {return m_rhs;}

void clear();

private:
std::vector<Double> m_value;
std::vector<CellID> m_row;
std::vector<CellID> m_column;
std::vector<Double> m_rhs;
std::unordered_map<std::pair<CellID, CellID>, std::size_t, pair_hash> m_indexMap;
};

#endif // TRIPLETSYSTEM_HPP