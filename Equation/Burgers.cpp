#include <cmath>
#include <numbers>  // Required for std::numbers

#include <Burgers.hpp>

/*CellScalarField Burgers::initialiseSolution(){
    auto& cell       = m_mesh.getCells();
    auto& cellCenter = m_mesh.getCellCenter();
    CellScalarField u(m_mesh, "u");
    
    for(CellID i : u.getData()){
        Double x = cellCenter[i];
        u[i] = std::sin(std::numbers::pi * x); 
    } 
    return u;
}*/

TridiagonalTerm Burgers::buildBurgers(){
    return buildAdvectionTerm() + buildViscousTerm() + buildTransientTerm();
}


Double Burgers::advanceTime(){
    return m_time + m_dt;
}