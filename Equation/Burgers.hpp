#ifndef BURGERS_HPP
#define BURGERS_HPP

#include <vector>

#include "CellScalarField.hpp"
#include "Equation.hpp"
#include "Expressions.hpp"
#include "FaceScalarField.hpp"
#include "Mesh.hpp"
#include "TripletSystem.hpp"
#include "Types.hpp"

enum BcType {Dirichlet, Neumann};

/**
 * @brief Solver for the 1D viscous Burgers' equation using the finite volume method.
 * 
 * This class assembles and solves the discretized form of the Burgers' equation over a structured 1D mesh.
 * It supports modular assembly of the advection, diffusion (viscous), and transient terms, and is designed
 * to work with implicit solvers. It serves as a physics-specific implementation derived from a generic Equation interface.
 */
class Burgers
{
public:
Burgers(Mesh& mesh, Double tFinal, Double dt, Double nu): 
                                           m_size(mesh.getNCells()),
                                           m_u(mesh, "u"),
                                           m_uf(mesh, "mdot"),
                                           m_tFinal(tFinal),
                                           m_dt(dt), 
                                           m_nu(nu){
                                           initialiseSolution(mesh);}
// System initialisation
void initialiseSolution(Mesh& mesh);

// Build functions
void buildBurgers(Mesh& mesh, TripletSystem& tripletSystem);
void buildAdvectionTerm(Mesh& mesh, TripletSystem& tripletSystem);
void buildViscousTerm(Mesh& mesh, TripletSystem& tripletSystem);
void buildTransientTerm(Mesh& mesh, TripletSystem& tripletSystem);
// For the moment, only periodic
void assignBc(Mesh& mesh);

// Solving process
CellScalarField solve(Double tol);
FaceScalarField updateFaceFlux();
Double advanceTime();
Double computeResiduals();
bool checkConvergence();

private:
Double m_size;
CellScalarField m_u;
FaceScalarField m_uf;

bool m_isConverged = false;
Double m_tFinal;
Double m_dt;
Double m_time = 0.0;
    
Double m_nu;

};

#endif // BURGERS_HPP