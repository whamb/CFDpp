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
    ScalarField(std::string name, size_t size): m_name(name) {
        m_field.resize(size);
    }

    Double& operator[](size_t i) { return m_field[i]; }
    const std::string getName() const { return m_name; }

private:
    std::vector<Double> m_field;
    std::string m_name;
};

#endif // SCALARFIELD_HPP