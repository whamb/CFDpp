#ifndef NODE_H
#define NODE_H

#include<iostream>

#include <Geometry.hpp>
class Node : public Geometry
{
public:
    Node() = delete;
    Node(const int id, const double x) : Geometry(id), m_x(x){}

    double getX() const {return m_x;}

    friend std::ostream& operator<<(std::ostream& os, const Node& node) 
    {
        os << "Node id =  " << node.getId() << ": x = " << node.getX() << "\n";
        return os;
    }

private:
    double m_x;
};

#endif //NODE_H