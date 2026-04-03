#include <assert.h>

#include <Mesh.hpp>
#include <TripletSystem.hpp>
#include <Types.hpp>

// Constructor: initialise RHS vector with number of cells
TripletSystem::TripletSystem(const Mesh& mesh){
    m_tripletRhs.resize(mesh.nCells()); // one RHS entry per cell
    m_lhs = std::make_unique<Lhs>(
            std::span<const Double> (m_value),
            std::span<const CellID> (m_row),
            std::span<const CellID> (m_column)
    );

    m_rhs = std::make_unique<Rhs>(
            std::span<const Double> (m_tripletRhs)
    );
}

// Add contribution to matrix (LHS) at position (row, column)
void TripletSystem::buildLHSMap(const CellID row, const CellID column, const Double value) {

    // Pack (row, column) into a single 64-bit key:
    uint64_t key = (static_cast<uint64_t>(row) << 32) | column;

    // Try to insert the key into the map:
    // - if key does not exist → inserted = true, value initialized to m_value.size()
    // - if key exists → inserted = false, iterator points to existing entry
    auto [it, inserted] = m_indexMap.try_emplace(key, m_value.size());

    if (inserted){
        // New (row, column) entry:
        m_value.push_back(value);   // matrix coefficient
        m_row.push_back(row);       // row index
        m_column.push_back(column); // column index
    } 
    else 
    {
        // Existing entry:
        m_value[it->second] += value;
    }
}

// Add contribution to matrix (LHS) at position (row, column)
void TripletSystem::addToLHS(const CellID row, const CellID column, const Double value) {

    // Pack (row, column) into a single 64-bit key:
    uint64_t key = (static_cast<uint64_t>(row) << 32) | column;

    // Existing entry:
    m_value[m_indexMap.at(key)] += value;
}

// Reset system for reuse
void TripletSystem::clear(){
    m_value.assign(m_value.size(), 0.0);    // clear matrix values

    // Reset RHS to zero but keep allocated memory (no reallocation)
    m_tripletRhs.assign(m_tripletRhs.size(), 0.0);

    // Clear index map (removes all key → index mappings)
    //m_indexMap.clear();
}