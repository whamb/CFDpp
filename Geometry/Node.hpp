#ifndef NODE_H
#define NODE_H

#include<iostream>

#include <Geometry.hpp>
#include "Types.hpp"
class Node : public Geometry
{
public:
    Node() = delete;
    Node(const NodeID id, const Double x) : Geometry(id), m_x(x){}

    Double getX() const {return m_x;}

    friend std::ostream& operator<<(std::ostream& os, const Node& node) 
    {
        os << "Node id =  " << node.getId() << ": x = " << node.getX() << "\n";
        return os;
    }

private:
    Double m_x;
};

#endif //NODE_H