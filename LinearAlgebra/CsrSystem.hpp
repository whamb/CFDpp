#ifndef CSRSYSTEM_HPP
#define CSRSYSTEM_HPP

#include <span>
#include <vector>

#include <LinearSystem.hpp>
#include <TripletSystem.hpp>

/**
 * @brief Concrete implementation of a sparse linear system using Compressed Sparse Row (CSR) format.
 * 
 * This class converts a `TripletSystem` (sparse matrix in triplet form) to CSR format,
 * and implements the `LinearSystem` interface for solver integration. 
 * It manages internal CSR storage (values, row pointers, and column indices),
 * and provides access to the LHS and RHS components of the system.
 * 
 * Key features:
 * - Efficient memory layout for sparse matrices.
 * - Ready for integration with iterative solvers (e.g. Jacobi, BiCGSTAB).
 * - Read-only views of matrix/vector via `std::span`.
 */
class CsrSystem
{
public:
    CsrSystem(const TripletSystem& tripletSystem)
    {   
        convertTripletToCsr(tripletSystem);
        m_lhs = std::make_unique<Lhs>(
            std::span<const Double> (m_values),
            std::span<const CellID> (m_compressedRow),
            std::span<const CellID> (m_columnIdx)        
        );

        m_rhs = std::make_unique<Rhs>(
            std::span<const Double> (m_csrRhs)
        );
    }

    const Lhs* lhs() const { return dynamic_cast<const Lhs*>(m_lhs.get()); }
    const Rhs* rhs() const { return dynamic_cast<const Rhs*>(m_rhs.get()); }

    CellID rhsSize() const { return m_csrRhs.size(); }
    CellID lhsSize() const { return m_values.size(); }
    CellID nRows()   const { return m_compressedRow.size() - 1; }

    const std::vector<Double>& values()        const { return m_values; }
    const std::vector<CellID>& columnIdx()     const { return m_columnIdx; }
    const std::vector<CellID>& compressedRow() const { return m_compressedRow; }
    const std::vector<Double>& csrRhs()        const { return m_csrRhs; }

private:
    std::unique_ptr<Lhs> m_lhs;
    std::unique_ptr<Rhs> m_rhs;
    std::vector<Double>  m_values;
    std::vector<CellID>  m_columnIdx;
    std::vector<CellID>  m_compressedRow;
    std::vector<Double>  m_csrRhs;

    void convertTripletToCsr(const TripletSystem& tripletSystem);
};

#endif //CSRSYSTEM