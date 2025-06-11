#ifndef FACE_h
#define FACE_h

#include <assert.h>
#include <array>

#include <Geometry.hpp>
#include <Node.hpp>

class Face : public Geometry
{
public:
    
Face() = delete;
Face(const int id, const Node& node) : Geometry(id), m_nodeId(node.getId()), m_center(node.getX()){
    assert(isValid() && "Invalid Face \n");
}

double getCenter() const {return m_center;}

bool isValid() const {
    return (m_nodeId >= 0);}

friend std::ostream& operator<<(std::ostream& os, Face& face){
    os << "Face id =  " << face.getId() << ": Node Id = " << face.m_nodeId <<  "\n";
    return os;
}

void addCell(const int cellId);

private:

int m_nodeId;
std::array<int,2> m_cellIds {-1,-1};
double m_center = 0.0;
double m_area = 1.0;
double m_normal = 1.0;

};

#endif // face_h