#ifndef EQUATION_HPP
#define EQUATION_HPP

#include "ScalarField.hpp"

class Equation 
{
public:

virtual void assemble() = 0;
virtual void solve(ScalarField& result) = 0;
};

#endif