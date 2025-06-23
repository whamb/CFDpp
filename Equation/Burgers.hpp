#ifndef BURGERS_HPP
#define BURGERS_HPP

#include <vector>

#include "CellScalarField.hpp"
#include "Equation.hpp"
#include <Expressions.hpp>
#include <FaceScalarField.hpp>
#include "Mesh.hpp"
#include "TridiagonalSystem.hpp"
#include "Types.hpp"

enum BcType {Dirichlet, Neumann};
struct TridiagonalTerm {
    TridiagonalLHS m_lhs;
    TridiagonalRHS m_rhs;

    TridiagonalTerm(const TridiagonalLHS& lhs, const TridiagonalRHS& rhs): m_lhs(lhs), m_rhs(rhs){};
    TridiagonalTerm operator+(const TridiagonalTerm& other){
        return(TridiagonalTerm(m_lhs + other.m_lhs, m_rhs + other.m_rhs)); 
    } 
};

/**
 * @brief Solver for the 1D viscous Burgers' equation using the finite volume method.
 * 
 * This class assembles and solves the discretized form of the Burgers' equation over a structured 1D mesh.
 * It supports modular assembly of the advection, diffusion (viscous), and transient terms, and is designed
 * to work with implicit solvers. It serves as a physics-specific implementation derived from a generic Equation interface.
 */
class Burgers : public Equation <TridiagonalLHS,TridiagonalRHS, CellScalarField> 
{
public:
    Burgers(Mesh& mesh, Double tFinal, Double dt, Double nu): 
                                               m_mesh(mesh),
                                               m_size(m_mesh.getNCells()),
                                               m_u(mesh, "u"),
                                               m_uf(mesh, "mdot"),
                                               m_tFinal(tFinal),
                                               m_dt(dt), 
                                               m_nu(nu){
                                               initialiseSolution();}

    CellScalarField initialiseSolution() override;

    TridiagonalTerm buildBurgers();
    TridiagonalTerm buildAdvectionTerm();
    TridiagonalTerm buildViscousTerm();
    TridiagonalTerm buildTransientTerm();
    TridiagonalTerm assignBc(BcType lBcType, Double lBc, BcType rBcType, Double rBc);

    CellScalarField solve(Double tol) override;
    FaceScalarField updateFaceFlux();
    Double advanceTime();

    Double computeResiduals();
    bool checkConvergence();

private:
    Mesh& m_mesh;
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