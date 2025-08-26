#include <petscksp.h>

#include <Constants.hpp>
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
    PetscInt n =   csrRhs->m_csrRhs.size();
    Vec b;
    VecCreateSeqWithArray(PETSC_COMM_SELF, 1, n, rhsVec.data(), &b);
    return b;
}

CellScalarField PetscCsrSolve::solveWithPETSc(const Mesh& mesh, const CsrSystem& system) {
    //PetscInitialize(NULL, NULL, NULL, NULL);

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
    KSPSetTolerances(ksp, PETSC_DEFAULT, PETSC_DEFAULT, PETSC_DEFAULT, maxIts);
    KSPSetFromOptions(ksp);
    KSPSolve(ksp, b, x);

    // Extract solution into std::vector
    CellScalarField solution(mesh, "u");
    const PetscScalar *xArray;
    VecGetArrayRead(x, &xArray);
    for (PetscInt i = 0; i < n; ++i) {
        solution[i] = static_cast<double>(xArray[i]);
    }
    VecRestoreArrayRead(x, &xArray);

    // Cleanup
    KSPDestroy(&ksp);
    VecDestroy(&x);
    VecDestroy(&b);
    MatDestroy(&A);

    //PetscFinalize();

    return solution; // Return solution vector
}