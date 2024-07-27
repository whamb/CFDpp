#ifndef CELL_h
#define CELL_h

#include <vector>
#include <array>

#include <Geo.hpp>
#include <Face.hpp>


class Cell : public Geo<Face>
{
public:

Cell() : Geo<Face>(), m_vol(0.0){}

Cell(const Face* f1, const Face* f2) : Geo<Face>()
{
    m_subGeo.push_back(f1);
    m_subGeo.push_back(f2);
    m_vol = vol();
}

const bool isValid() const;

const double vol() const;

private:

double m_vol{0};

};

#endif // cell_h