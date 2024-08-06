#include <vector>
#include <array>
#include <cassert>

#include <Cell.hpp>
#include <Face.hpp>
#include <Geo.hpp>


const bool Cell::isValid() const
{
    return(getNSubGeos() == 2 ? true : false);
}

const double Cell::getVol() const
{
    return m_vol;
}

void Cell::setVol(double vol)
{
    m_vol = vol;
}