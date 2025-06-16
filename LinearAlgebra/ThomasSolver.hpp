#ifndef THOMASSOLVER_HPP
#define THOMASSOLVER_HPP

#include "LinearSolver.hpp"

class ThomasSolver : public LinearSolver
{
public:
virtual std::vector<Double> solve(LHS& A, RHS& b) override;
};

#endif //THOMASSOLVER