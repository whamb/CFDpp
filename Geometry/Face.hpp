#ifndef FACE_h
#define FACE_h

#include <vector>
#include <array>

#include <GeoComplex.hpp>
#include <Node.hpp>

class Face : public GeoComplex
{
public:

    Face() : GeoComplex(), m_surf(0.0){}

    Face(const Node& node1, const Node& node2)
    {
        getSubGeoIds().push_back(node1.getId());
        getSubGeoIds().push_back(node2.getId());
        m_surf = setSurf(node1, node2);
    }

    const bool isValid() const;

    const double setSurf(const Node& node1, const Node& node2) const;
    const double getSurf() const;

    friend std::ostream& operator<<(std::ostream& os, Face& face) 
    {
            os << "Face id =  " << face.getId() << " Node0 Id = " << (face.getSubGeoIds())[0] <<", Node1 Id = " << (face.getSubGeoIds())[1] << std::endl;
            return os;
    }

private:

double m_surf;

};

#endif // face_h