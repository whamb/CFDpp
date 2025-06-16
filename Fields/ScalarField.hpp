#ifndef SCALARFIELD_HPP
#define SCALARFIELD_HPP

#include <vector>

#include <Types.hpp>

class ScalarField
{
public:
    ScalarField(std::string name, size_t size): m_name(name){
        m_field.resize(size);
    }
    Double& operator[](size_t i){return m_field[i];};
    const std::string getName() const {return m_name;}

private:
    /* data */
    std::vector<Double> m_field;
    std::string m_name;
};




#endif // SCALARFIELD_HPP