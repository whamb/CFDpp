#include <vector>
#include <array>
#include <math.h>
#include <cassert>

#include <Face.hpp>

const bool Face::isValid() const
{
    return(getNSubGeos() == 2 ? true : false);
}

const double Face::setSurf(const Node& node1, const Node& node2) const
{
    assert(("Face does not have 2 nodes", isValid()));
    return sqrt((node2.Node::getX() - node1.Node::getX()) * (node2.Node::getX() - node1.Node::getX()) 
            +  (node2.Node::getY() - node1.Node::getY()) * (node2.Node::getY() - node1.Node::getY()));  
}

const double Face::getSurf() const
{
    return m_surf;
}