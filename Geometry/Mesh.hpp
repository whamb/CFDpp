#ifndef MESH_H
#define MESH_H

#include <cassert>

#include "Cell.hpp"
#include "Domain.hpp"
#include "Face.hpp"
#include "Node.hpp"


class Mesh
{

public:
Mesh() : m_lowerBound(0), m_upperBound(0), m_dx(0){}

private:

double m_lowerBound;
double m_upperBound;
double m_dx;

};

#endif