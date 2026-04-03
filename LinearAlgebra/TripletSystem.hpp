#ifndef TRIPLETSYSTEM_HPP
#define TRIPLETSYSTEM_HPP

#include <span>
#include <vector>
#include <unordered_map>

#include <LinearSystem.hpp>
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
    TripletSystem(const Mesh& mesh);

    const Lhs* lhs() const { return dynamic_cast<const Lhs*>(m_lhs.get()); }
    const Rhs* rhs() const { return dynamic_cast<const Rhs*>(m_rhs.get()); }

    void buildLHSMap(const CellID row, const CellID column, const Double value);
    void addToLHS   (const CellID row, const CellID column, const Double value);
    void addToRHS   (const CellID row, const Double value) {m_tripletRhs[row] += value;}
    void setRHS     (const CellID row, const Double value) {m_tripletRhs[row] = value;}

    CellID rhsSize() const {return m_tripletRhs.size();}
    CellID lhsSize() const {return m_value.size();}

    const std::vector<Double>& values()     const {return m_value;}
    const std::vector<CellID>& rows()       const {return m_row;}
    const std::vector<CellID>& columns()    const {return m_column;}
    const std::vector<Double>& tripletRhs() const {return m_tripletRhs;}

    void clear();

private:
    std::unique_ptr<Lhs> m_lhs;
    std::unique_ptr<Rhs> m_rhs;
    std::vector<Double> m_value;
    std::vector<CellID> m_row;
    std::vector<CellID> m_column;
    std::vector<Double> m_tripletRhs;
    std::unordered_map<uint64_t, std::size_t> m_indexMap;
};

#endif // TRIPLETSYSTEM_HPP