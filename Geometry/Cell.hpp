#ifndef CELL_h
#define CELL_h

#include <vector>
#include <array>

#include <GeoComplex.hpp>
#include <Face.hpp>


class Cell : public GeoComplex
{
public:

Cell() : GeoComplex(), m_vol(0.0){}

Cell(const Face& f1, const Face& f2)
{
    getSubGeoIds().push_back(f1.getId());
    getSubGeoIds().push_back(f2.getId());
}

const bool isValid() const;

void setVol(double vol);
const double getVol() const;

friend std::ostream& operator<<(std::ostream& os, Cell& cell) 
    {
            os << "Cell id =  " << cell.getId() << " Face0 Id = " << (cell.getSubGeoIds())[0] <<", Node1 Id = " << (cell.getSubGeoIds())[1] << std::endl;
            return os;
    }

private:

double m_vol;

};

#endif // cell_h