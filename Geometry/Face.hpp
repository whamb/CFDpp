#ifndef FACE_h
#define FACE_h

#include <vector>
#include <array>

#include <Geo.hpp>
#include <Node.hpp>

class Face : public Geo<Node>
{
public:

Face() : Geo<Node>(), m_surf(0.0){}

Face(const Node* n1, const Node* n2) : Geo<Node>()
{
    m_subGeo.push_back(n1);
    m_subGeo.push_back(n2);
    m_surf = surf();
}

const bool isValid() const;

const double surf() const;

private:

double m_surf{0};

};

#endif // face_h