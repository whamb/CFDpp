#ifndef SCALARFIELD_HPP
#define SCALARFIELD_HPP

#include <vector>

#include <Types.hpp>

/**
 * @brief Represents a named scalar field over a 1D mesh.
 * 
 * Provides storage and indexed access to field values, along with a label for identification.
 * Used to store solutions (e.g., velocity, pressure) or intermediate quantities in PDE solvers.
 */
class ScalarField
{
public:

    ScalarField(const std::string name, const std::size_t size) : m_name(std::move(name)), m_field(size)
    {};
    
    const std::vector<Double>&    data() const { return m_field; }
    std::size_t                   size() const { return m_field.size(); }
    std::string_view              name() const { return m_name; }
    Double&                       operator[](size_t i) { return m_field[i]; }
    const Double&                 operator[](size_t i) const { return m_field[i]; }

private:
    std::string m_name;
    std::vector<Double> m_field;
};

#endif // SCALARFIELD_HPP