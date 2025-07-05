#include <TripletSystem.hpp>
#include <CsrSystem.hpp>
#include <Mesh.hpp>
#include <PetscCsrSolve.hpp>

// A mock class to expose protected members for test validation (optional)
class CsrSystemTestable : public CsrSystem {
public:
    using CsrSystem::CsrSystem;
    const std::vector<Double>& getValues() const { return m_value; }
    const std::vector<CellID>& getColumnIdx() const { return m_columnIdx; }
    const std::vector<CellID>& getCompressedRow() const { return m_compressedRow; }
    const std::vector<Double>& getCsrRhs() const { return m_csrRhs; }
};

int main(){
    // Build a dummy mesh with 3 cells
    Mesh mesh(0.0, 3.0, 1.0); // Will create 3 cells: [0,1], [1,2], [2,3]

    TripletSystem triplet(mesh);
    // Matrix:
    // [ 2 3 0 ]
    // [ 3 5 4 ]
    // [ 0 4 5 ]
    triplet.addToLHS(2.0, 0, 0);
    triplet.addToLHS(5.0, 2, 2);
    triplet.addToLHS(8.0, 1, 1);
    //triplet.addToLHS(2.0, 0, 0);
    //triplet.addToLHS(3.0, 0, 1);
    //triplet.addToLHS(3.0, 1, 0);
    //triplet.addToLHS(4.0, 1, 2);
    //triplet.addToLHS(4.0, 2, 1);
    //triplet.addToLHS(5.0, 2, 2);
    //triplet.addToLHS(5.0, 1, 1);

    // RHS = [1, 2, 3]
    triplet.addToRHS(0, 1.0);
    triplet.addToRHS(1, 2.0);
    triplet.addToRHS(2, 3.0);

    // Convert to CSR
    CsrSystemTestable csr(triplet);

    // Linear Solve test
    PetscCsrSolve::solveWithPETSc(csr);
}