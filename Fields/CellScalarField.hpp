#ifndef CELLSCALARFIELD_HPP
#define CELLSCALARFIELD_HPP

#include <vector>
#include <ScalarField.hpp>

class CellScalarField : public ScalarField<std::vector<Double>>
{
public:
CellScalarField(const Mesh& mesh, std::string name): ScalarField(name){
    m_field.resize(mesh.getNCells());}
};

#endif //CELLSCALARFIELD