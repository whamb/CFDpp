#ifndef LINEARSOLVER_HPP
#define LINEARSOLVER_HPP

#include <vector>

#include "LinearSystem.hpp"

class LinearSolver {
public:
virtual std::vector<Double> solve(LHS& A, RHS& b) = 0;
};

#endif //LINEARSOLVER