#include <petscksp.h>

#include <Constants.hpp>
#include <CsrSystem.hpp>
#include <PetscCsrSolve.hpp>
#include <ScalarField.hpp>

Mat PetscCsrSolve::setupCsrLhs(const Lhs* csrLhs){
    const auto& rowPtr    = csrLhs->m_row; // size n+1
    const auto& colIdxPtr = csrLhs->m_column;     // size nnz
    const auto& valuesPtr = csrLhs->m_value;        // size nnz

    PetscInt n = csrLhs->m_row.size() - 1;           // num rows
    PetscInt* ia   = const_cast<PetscInt*>(rowPtr.data());
    PetscInt* ja   = const_cast<PetscInt*>(colIdxPtr.data());
    PetscScalar* a = const_cast<PetscScalar*>(valuesPtr.data());

    // Wrap CSR into PETSc matrix
    Mat A;
    MatCreateSeqAIJWithArrays(PETSC_COMM_SELF, n, n, ia, ja, a, &A);
    return A;
}

Mat PetscCsrSolve::setupTripletLhs(const TripletSystem& tripletSystem){
    // Wrap CSR into PETSc matrix
    const PetscInt nRows = tripletSystem.rhsSize();

    Mat A;
    MatCreate(PETSC_COMM_WORLD, &A); 
    MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, nRows, nRows);
    MatSetFromOptions(A);

    // 1D FVM burgers: 3 nonzeros per row
    MatSeqAIJSetPreallocation(A, 5, NULL);

    MatZeroEntries(A);

    for (size_t i = 0; i < tripletSystem.lhsSize(); ++i) {
        PetscInt row    = tripletSystem.rows()[i];
        PetscInt col    = tripletSystem.columns()[i];
        PetscScalar val = tripletSystem.values()[i];

        MatSetValue(A, row, col, val, ADD_VALUES);
    }

    MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
    return A;
}

Vec PetscCsrSolve::setupRhs(const Rhs* rhs){
    auto& rhsVec = rhs->m_rhs;        // size n
    PetscInt n =   rhs->m_rhs.size();
    Vec b;
    VecCreateSeqWithArray(PETSC_COMM_SELF, 1, n, rhsVec.data(), &b);
    return b;
}

ScalarField PetscCsrSolve::solveWithPETSc(const Mesh& mesh, const CsrSystem& system) {
    //PetscInitialize(NULL, NULL, NULL, NULL);

    PetscInt n = system.nRows();          

    // Wrap CSR into PETSc matrix
    Mat A = setupCsrLhs(system.lhs());

    // Create vectors
    Vec b = setupRhs(system.rhs());
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
    ScalarField solution("u", mesh.nCells());
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

ScalarField PetscCsrSolve::solveWithPETSc(const Mesh& mesh, const TripletSystem& system) {
    //PetscInitialize(NULL, NULL, NULL, NULL);

    PetscInt n = system.rhsSize();         

    // Wrap CSR into PETSc matrix
    Mat A = setupTripletLhs(system);

    // Create vectors
    Vec b = setupRhs(system.rhs());
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
    ScalarField solution("u", system.rhsSize());
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