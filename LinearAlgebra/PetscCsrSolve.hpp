#ifndef PETSCCSRSOLVE_HPP
#define PETSCCSRSOLVE_HPP

#include <petscksp.h>

#include <CsrSystem.hpp>

/**
 * @brief Interface to solve a CSR-formatted linear system using PETSc.
 * 
 * This class provides a static method that wraps PETSc routines to solve
 * a system of the form A·x = b using the matrix and vector provided by
 * `CsrSystem`. Assumes PETSc has already been initialized by the caller.
 */
class PetscCsrSolve
{
public:
/**
 * @brief Solve the given CSR linear system using PETSc routines.
 * 
 * @param system A reference to the CSR system to be solved. Must have valid matrix and RHS.
 */
static CellScalarField solveWithPETSc(const Mesh& mesh, const CsrSystem& system);

static Mat setupCsrLhs(const CsrLHS* csrLhs);
static Vec setupCsrRhs(const CsrRHS* csrLhs);


};

#endif // PETSCCSRSOLVE_HPP