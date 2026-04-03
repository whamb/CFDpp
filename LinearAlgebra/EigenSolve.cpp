#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Core>
#include <Eigen/Sparse>

#include <CsrSystem.hpp>
#include <EigenSolve.hpp>
#include <Mesh.hpp>
#include <ScalarField.hpp>

// Solve linear system Ax = b using Eigen iterative solver
ScalarField EigenSolve::solveWithEigen(const Mesh& mesh, const TripletSystem& tripletSystem){

    // Build sparse matrix A from triplet system (COO → Eigen format)
    const Eigen::SparseMatrix<Double> A = EigenSolve::setupLhs(tripletSystem);

    // Map RHS vector into Eigen vector (no copy)
    const Eigen::VectorXd b = EigenSolve::setupRhs(tripletSystem);

    // BiCGSTAB solver: suitable for non-symmetric sparse systems (like Burgers)
    Eigen::BiCGSTAB<Eigen::SparseMatrix<Double>> solver;

    // Precompute internal structures (factorization / preconditioner setup)
    solver.compute(A);

    // Solve Ax = b
    Eigen::VectorXd x = solver.solve(b);

    // Wrap solution into your ScalarField abstraction
    // Copies data from Eigen vector into ScalarField
    ScalarField solution ("u", {x.data(), x.data() + x.size()});

    return solution;
}

// Convert TripletSystem (COO format) into Eigen sparse matrix
Eigen::SparseMatrix<Double> EigenSolve::setupLhs(const TripletSystem& tripletSystem){

    // Access raw triplet data (zero-copy views)
    const std::span<const CellID> rows   = tripletSystem.rows();
    const std::span<const CellID> cols   = tripletSystem.columns();
    const std::span<const Double> values = tripletSystem.values();

    // Eigen requires its own triplet structure
    std::vector<Eigen::Triplet<double>> triplets;

    CellID lhsSize = tripletSystem.lhsSize(); // number of non-zero entries
    CellID rhsSize = tripletSystem.rhsSize(); // matrix dimension

    triplets.reserve(lhsSize); // avoid reallocations

    // Convert custom triplets → Eigen triplets
    for (size_t i = 0; i < lhsSize; ++i){
        triplets.emplace_back(rows[i], cols[i], values[i]);
    }

    // Create sparse matrix
    Eigen::SparseMatrix<Double> A;

    // Matrix is square: (nCells x nCells)
    A.resize(rhsSize, rhsSize);

    // Build matrix from triplets
    // Eigen will internally:
    // - sort entries
    // - merge duplicates (sum values with same (i,j))
    A.setFromTriplets(triplets.begin(), triplets.end());

    return A;
}

// Create Eigen vector view of RHS (zero-copy)
Eigen::Map<const Eigen::VectorXd> EigenSolve::setupRhs(const TripletSystem& tripletSystem){

    // Map raw pointer to Eigen vector without copying data
    return Eigen::Map<const Eigen::VectorXd>(
        tripletSystem.tripletRhs().data(), // pointer to data
        tripletSystem.rhsSize()     // size of vector
    );
}

