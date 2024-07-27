#ifndef MESH_H
#define MESH_H

#include <cassert>

#include <Cell.hpp>
#include <Domain.hpp>
#include <Face.hpp>
#include <Node.hpp>


class Mesh
{

public:
Mesh() : m_lowerBound(0), m_upperBound(0), m_dx(0){}

Mesh(double lowerBound, double upperBound, double dx, Domain& domain) : m_lowerBound(lowerBound), m_upperBound(upperBound), m_dx(dx)
{
    assert(("Invalid mesh", m_lowerBound < m_upperBound || m_dx > 0));
    //createNodes(domain);
    //createFaces(domain);
    //createCells(domain);
}

void createNodes(Domain& domain);
void createFaces(Domain& domain);
void createCells(Domain& domain);

private:
double m_lowerBound;
double m_upperBound;
double m_dx;

};

#endif