#ifndef SOLUTION_h
#define SOLUTION_h

#include <memory>
#include <vector>
#include <array>

#include <Face.hpp>
#include <Cell.hpp>
#include <Domain.hpp>
#include <Scalar.hpp>

class Solution : Scalar
{

public:

Solution(Domain& domain);

void initialise(std::vector<double> solution);

private:
    
};

#endif // SOLUTION_h