#include <petscksp.h>

#include <CsrSystem.hpp>
#include <PetscCsrSolve.hpp>

Mat PetscCsrSolve::setupCsrLhs(const CsrLHS* csrLhs){
    auto& rowPtr = csrLhs->m_compressedRow; // size n+1
    auto& colIdx = csrLhs->m_columnIdx;     // size nnz
    auto& values = csrLhs->m_value;        // size nnz

    PetscInt n = csrLhs->m_compressedRow.size() - 1;           // num rows
    PetscInt* ia = const_cast<PetscInt*>(rowPtr.data());
    PetscInt* ja = const_cast<PetscInt*>(colIdx.data());
    PetscScalar* a = const_cast<PetscScalar*>(values.data());

    // Wrap CSR into PETSc matrix
    Mat A;
    MatCreateSeqAIJWithArrays(PETSC_COMM_SELF, n, n, ia, ja, a, &A);
    return A;
}

Vec PetscCsrSolve::setupCsrRhs(const CsrRHS* csrRhs){
    auto& rhsVec = csrRhs->m_csrRhs;        // size n
    PetscInt n =   csrRhs->m_csrRhs.size() - 1;
    Vec b;
    VecCreateSeqWithArray(PETSC_COMM_SELF, 1, n, rhsVec.data(), &b);
    return b;
}

void PetscCsrSolve::solveWithPETSc(const CsrSystem& system) {
    PetscInitialize(NULL, NULL, NULL, NULL);

    PetscInt n = system.getNRows();          
    // Wrap CSR into PETSc matrix
    Mat A = setupCsrLhs(system.getLhs());
    // Create vectors
    Vec b = setupCsrRhs(system.getRhs());
    Vec x;
    VecCreateSeq(PETSC_COMM_SELF, n, &x);

    // Solve
    KSP ksp;
    KSPCreate(PETSC_COMM_SELF, &ksp);
    KSPSetOperators(ksp, A, A);
    KSPSetFromOptions(ksp);
    KSPSolve(ksp, b, x);

    // Output solution
    VecView(x, PETSC_VIEWER_STDOUT_WORLD);

    // Cleanup
    KSPDestroy(&ksp);
    VecDestroy(&x); VecDestroy(&b); MatDestroy(&A);
    PetscFinalize();
}