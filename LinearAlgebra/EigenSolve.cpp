#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Core>
#include <Eigen/Sparse>

#include <CsrSystem.hpp>
#include <EigenSolve.hpp>
#include <Mesh.hpp>
#include <ScalarField.hpp>

ScalarField EigenSolve::solveWithEigen(const Mesh& mesh, const TripletSystem& tripletSystem){
    const Eigen::SparseMatrix<Double> A = EigenSolve::setupLhs(tripletSystem);
    const Eigen::VectorXd b             = EigenSolve::setupRhs(tripletSystem);

    Eigen::BiCGSTAB<Eigen::SparseMatrix<Double>> solver;
    solver.compute(A);
    Eigen::VectorXd x = solver.solve(b);

    ScalarField solution ("u", {x.data(), x.data() + x.size()});
    return solution;
}

Eigen::SparseMatrix<Double> EigenSolve::setupLhs(const TripletSystem& tripletSystem){
    const std::span<const CellID> rows    = tripletSystem.rows();
    const std::span<const CellID> cols    = tripletSystem.columns();
    const std::span<const Double> values  = tripletSystem.values();

    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(rows.size());

    for (size_t i = 0; i < rows.size(); ++i){
        triplets.emplace_back(rows[i], cols[i], values[i]);
    }

    Eigen::SparseMatrix<double> A;
    A.setFromTriplets(triplets.begin(), triplets.end());
    return A;
};

Eigen::Map<const Eigen::VectorXd> EigenSolve::setupRhs(const TripletSystem& tripletSystem){
    return Eigen::Map<const Eigen::VectorXd>(tripletSystem.rhs().data(),
                                             tripletSystem.rhs().size());
};

