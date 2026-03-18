#ifndef EIGENSOLVE_HPP
#define EIGENSOLVE_HPP

#include <Eigen/Sparse>

#include <TripletSystem.hpp>
#include <Mesh.hpp>
#include <ScalarField.hpp>

class EigenSolve{
public:
    static ScalarField solveWithEigen(const Mesh& mesh, const TripletSystem& system);

    static Eigen::SparseMatrix<Double>       setupLhs(const TripletSystem& tripletSystem);
    static Eigen::Map<const Eigen::VectorXd> setupRhs(const TripletSystem& tripletSystem);
};

#endif // EIGENSOLVE_HPP