#ifndef CSRSYSTEM_HPP
#define CSRSYSTEM_HPP

#include <span>
#include <vector>

#include <CellScalarField.hpp>
#include <LinearSystem.hpp>
#include <LinSolverType.hpp>
#include <TripletSystem.hpp>

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

struct CsrLHS : public LHS{
std::span<const Double> m_value;
std::span<const CellID> m_columnIdx;
std::span<const CellID> m_compressedRow;

CsrLHS(std::span<const Double> value,
       std::span<const CellID> columnIdx,
       std::span<const CellID> compressedRow) : m_value(value),
                                                m_columnIdx(columnIdx),
                                                m_compressedRow(compressedRow){};
};

/**
 * @brief Represents the right-hand side (RHS) vector of a CSR linear system.
 * 
 * This struct wraps a constant view of the right-hand side vector used in solving
 * linear systems of the form Ax = b.
 */

struct CsrRHS : public RHS{
std::span<const Double> m_csrRhs;

CsrRHS(std::span<const Double> rhs) : m_csrRhs(rhs){};
};

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
class CsrSystem : public LinearSystem
{
public:
CsrSystem(TripletSystem& tripletSystem): LinearSystem(nullptr, nullptr)
{   
    convertTripletToCsr(tripletSystem);
    m_lhs = std::make_unique<CsrLHS>(
        std::span<const Double>(m_value),
        std::span<const CellID>(m_columnIdx),
        std::span<const CellID>(m_compressedRow)
    );
    
    m_rhs = std::make_unique<CsrRHS>(
        std::span<const Double> (m_csrRhs)
    );
}

const CsrLHS* getLhs() const {return dynamic_cast<const CsrLHS*>(m_lhs.get());}
const CsrRHS* getRhs() const {return dynamic_cast<const CsrRHS*>(m_rhs.get());}

const CellID rhsSize() const {return m_csrRhs.size();}
const CellID lhsSize() const {return m_value.size();}
const CellID getNRows() const {return m_compressedRow.size() - 1;}

private:
void convertTripletToCsr(TripletSystem& tripletSystem);

//CellScalarField solveLinSystem(double linTol, LinSolverType linSolver) override;

protected:
std::vector<Double> m_value;
std::vector<CellID> m_columnIdx;
std::vector<CellID> m_compressedRow;
std::vector<Double> m_csrRhs;

};

#endif //CSRSYSTEM