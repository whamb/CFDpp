#ifndef NODE_H
#define NODE_H

class Node
{
public:
    Node() = delete;

    Node(double x, double y=0) : m_x(x), m_y(y)
    {}

    int getX() const;

    int getY() const;

private:
    double m_x,m_y;
};

#endif //NODE_H