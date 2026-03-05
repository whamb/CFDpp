#ifndef BURGERSEQN_HPP
#define BURGERSEQN_HPP

#include <vector>

#include <Expressions.hpp>
#include <ScalarField.hpp>
#include <Mesh.hpp>
#include <TripletSystem.hpp>
#include <Types.hpp>

enum BcType {Dirichlet, Neumann};

/**
 * @brief Solver for the 1D viscous Burgers' equation using the finite volume method.
 * 
 * This class assembles and solves the discretized form of the Burgers' equation over a structured 1D mesh.
 * It supports modular assembly of the advection, diffusion (viscous), and transient terms, and is designed
 * to work with implicit solvers. It serves as a physics-specific implementation derived from a generic Equation interface.
 */
class BurgersEqn
{
public:
    BurgersEqn(Mesh& mesh, Double dt, Double nu): 
        m_u("u", mesh.nCells()),
        m_uf("mdot", mesh.nFaces()),
        m_dt(dt), 
        m_nu(nu)
    {}
    // System initialisation
    void initialiseSolution(const Mesh& mesh);

    // Build functions
    void buildBurgers       (const Mesh& mesh, TripletSystem& tripletSystem);
    void buildAdvectionTerm (const Mesh& mesh, TripletSystem& tripletSystem);
    void buildViscousTerm   (const Mesh& mesh, TripletSystem& tripletSystem);
    void buildTransientTerm (const Mesh& mesh, TripletSystem& tripletSystem);
    
    // For the moment, only periodic
    void updateBc      (const Mesh& mesh, TripletSystem& tripletSystem);
    void updateFaceFlux(const Mesh& mesh);

    ScalarField& u(){return m_u;}

private:
    ScalarField m_u;
    ScalarField m_uf;
    Double m_dt;
    Double m_nu;
};

#endif // BURGERSEQN_HPP