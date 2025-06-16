#ifndef BURGERS_HPP
#define BURGERS_HPP

#include <vector>

#include "Equation.hpp"
#include "Mesh.hpp"
#include "ScalarField.hpp"
#include "Types.hpp"

/**
 * @brief Solver for the 1D viscous Burgers' equation using the finite volume method.
 * 
 * This class assembles and solves the discretized form of the Burgers' equation over a structured 1D mesh.
 * It supports modular assembly of the advection, diffusion (viscous), and transient terms, and is designed
 * to work with implicit solvers. It serves as a physics-specific implementation derived from a generic Equation interface.
 */
class Burgers : public Equation 
{
public:
    Burgers(Mesh& mesh);

    std::vector<Double> assembleUpperLHS();
    std::vector<Double> assembleLowerLHS();
    std::vector<Double> assembleDiagonalLHS();
    std::vector<Double> assembleRHS();

    std::vector<Double> buildAdvectionTerm(std::vector<Double>& massFlux);
    std::vector<Double> buildViscousTerm(double nu);
    std::vector<Double> buildTransientTerm(double dt);

    std::vector<Double> solveBurgers(Double tol);
    std::vector<Double> updateMassFlux(std::vector<Double>& u);

    Double computeResiduals();
    bool checkConvergence();

private:
    Mesh& m_mesh;
    bool m_isConverged = false;
};

#endif // BURGERS_HPP