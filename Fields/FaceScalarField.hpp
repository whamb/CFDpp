#ifndef FACESCALARFIELD_HPP
#define FACESCALARFIELD_HPP

#include <vector>
#include <ScalarField.hpp>

class FaceScalarField : public ScalarField<std::vector<Double>>
{
public:
FaceScalarField(Mesh& mesh, std::string_view name): ScalarField(name){
m_field.resize(mesh.getNFaces());};
};

#endif //FACESCALARFIELD