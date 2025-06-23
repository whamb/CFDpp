#ifndef SCALARFIELD_HPP
#define SCALARFIELD_HPP

#include <vector>

#include <Mesh.hpp>
#include <Types.hpp>

/**
 * @brief Represents a named scalar field over a 1D mesh.
 * 
 * Provides storage and indexed access to field values, along with a label for identification.
 * Used to store solutions (e.g., velocity, pressure) or intermediate quantities in PDE solvers.
 */
template<typename Container = std::vector<Double>>
class ScalarField
{
public:
ScalarField(const std::string_view name) : m_name(name){};

Container& getData(){return m_field;}
auto& operator[](size_t i) { return m_field[i]; }

CellID getSize() const {return m_field.size();}
const std::string getName() const { return m_name; }

~ScalarField() = default;

protected: 
Container m_field;

private:
std::string m_name;
};

#endif // SCALARFIELD_HPP