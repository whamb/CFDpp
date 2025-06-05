#ifndef SCALAR_h
#define SCALAR_h

#include <memory>
#include <vector>
#include <array>

#include <Domain.hpp>

class Scalar
{
    
public:

virtual void initialise(const std::vector<double>& initialScalarField) = 0;

std::vector<double>& getScalarField();

private:

std::vector<double> m_scalarField;
    
};

#endif // SCALAR_h