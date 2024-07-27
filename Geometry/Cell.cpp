#include <vector>
#include <array>
#include <cassert>

#include <Face.hpp>
#include <Geo.hpp>
#include <Cell.hpp>

const bool Cell::isValid() const
{
    return(getNGeos() == 2 ? true : false);
}

const double Cell::vol() const
{
    assert(("Cell does not have 2 faces", !isValid()));
    return(abs(getGeo(0) -> getGeo(0) -> getX() - getGeo(1) -> getGeo(0) -> getX()));
}