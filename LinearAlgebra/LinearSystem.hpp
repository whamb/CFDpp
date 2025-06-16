#ifndef LINEARSYSTEM_HPP
#define LINEARSYSTEM_HPP

#include <span>
#include <vector>

#include <Types.hpp>

class LHS {
public:
virtual ~LHS() = default;
virtual Double& operator()(CellID i, CellID j) = 0;
};

class RHS {
public:
virtual ~RHS() = default;
virtual Double& operator()(CellID i) = 0;
};

class LinearSystem {
public:
LinearSystem(std::unique_ptr<LHS> A, std::unique_ptr<RHS> b)
    : m_A(std::move(A)), m_b(std::move(b)) {}
Double& lhs(CellID i, CellID j) { return (*m_A)(i, j); }
Double& rhs(CellID i) { return (*m_b)(i); }

private:
std::unique_ptr<LHS> m_A;
std::unique_ptr<RHS> m_b;
};

#endif //LINEARSYSTEM