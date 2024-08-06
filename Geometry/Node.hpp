#ifndef NODE_H
#define NODE_H

#include<iostream>

#include <Geo.hpp>
class Node : public Geo
{
public:
    Node() = delete;
    Node(const double x, const double y=0) : m_x(x), m_y(y){}

    double getX() const;
    double getY() const;

    friend std::ostream& operator<<(std::ostream& os, const Node& node) 
    {
        os << "Node id =  " << node.getId() << " x = " << node.getX() <<", y = " << node.getY() << std::endl;
        return os;
    }

private:
    double m_x,m_y;
};

#endif //NODE_H