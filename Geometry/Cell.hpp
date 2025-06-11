#ifndef CELL_h
#define CELL_h

#include <array>
#include <math.h>
#include <vector>


#include <Geometry.hpp>
#include <Face.hpp>

class Cell : public Geometry
{
public:

Cell() = delete;

Cell(const int id, Face& f1, Face& f2) : 
Geometry(id), m_faceIds({f1.getId(),f2.getId()}), m_vol(std::abs(f1.getCenter() - f2.getCenter())), 
m_center(0.5*(f1.getCenter() - f2.getCenter())){
    assert(isValid() && "Invalid Cell \n");
    f1.addCell(id);
    f2.addCell(id);
}

const double getVol() const{return m_vol;}
const std::array<int, 2>& getFaceIds() const { return m_faceIds; }

bool isValid() const{
    return(m_faceIds[0]!=-1 && m_faceIds[1]!=-1 && m_vol > 0);
}

friend std::ostream& operator<<(std::ostream& os, Cell& cell) {
    os << "Cell id =  " << cell.getId() << ": Face0 Id = " << cell.m_faceIds[0] << ", Face1 Id = " << cell.m_faceIds[1] << "\n";
    return os;
}

private:

double m_vol = -1.0;
double m_center = -1.0;
std::array<int,2> m_faceIds {-1,-1};

};

#endif // cell_h