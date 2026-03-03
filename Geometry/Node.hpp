#ifndef NODE_H
#define NODE_H

#include <Types.hpp>

/**
 * @brief Represents a mesh node in 1D space.
 * 
 * A Node holds a spatial coordinate and a unique ID inherited from the Geometry base class.
 * Nodes serve as geometric anchors for defining faces in a finite volume mesh.
 */
class Node
{
public:
    Node() {m_id = -1;}
    Node(const NodeID id, const Double x) : m_id(id), m_x(x)
    {}

    NodeID id() const { return m_id; }
    Double x()  const { return m_x; }

private:
    NodeID m_id;
    Double m_x;
};

#endif // NODE_H