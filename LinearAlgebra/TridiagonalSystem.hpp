#ifndef TRIDIAGONALSYSTEM_HPP
#define TRIDIAGONALSYSTEM_HPP

#include <vector>

#include <LinearSystem.hpp>
#include <Types.hpp>

class TridiagonalLHS : public LHS{
public:
TridiagonalLHS(std::span<Double> lower,
               std::span<Double> upper,
               std::span<Double> diagonal,
               CellID size) : 
               m_lower(lower), 
               m_upper(upper), 
               m_diagonal(diagonal),
               m_size(size){};

Double& operator()(CellID i, CellID j) override;

private:
std::span<Double> m_lower;
std::span<Double> m_diagonal;
std::span<Double> m_upper;
CellID m_size;
};

class TridiagonalRHS : public RHS{
public:

TridiagonalRHS(std::span<Double> b,
               CellID size) : 
               m_b(b),
               m_size(size){};

Double& operator()(CellID i) override;

private:
std::span<Double> m_b;
CellID m_size;
};

class TridiagonalSystem : public LinearSystem{
public:
TridiagonalSystem(CellID size): 
          m_size(size),
          m_lower(size - 1,0),
          m_diagonal(size,0),
          m_upper(size - 1,0),
          m_b(size,0),
          LinearSystem(
              std::make_unique<TridiagonalLHS>(
                  std::span<Double>(m_lower),
                  std::span<Double>(m_upper),
                  std::span<Double>(m_diagonal),
                  size),
              std::make_unique<TridiagonalRHS>(
                  std::span<Double>(m_b),
                  size)) {}

std::vector<Double> thomasSolve() const;

private:

int m_size;
std::vector<Double> m_lower;
std::vector<Double> m_diagonal;
std::vector<Double> m_upper;
std::vector<Double> m_b;

};

#endif