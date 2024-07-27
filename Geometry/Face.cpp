#include <vector>
#include <array>
#include <math.h>
#include <cassert>

#include <Face.hpp>
#include <Geo.hpp>
#include <Cell.hpp>

const bool Face::isValid() const
{
    return(getNGeos() == 2 ? true : false);
}

const double Face::surf() const
{
    assert(("Face does not have 2 nodes", !isValid()));
    return sqrt((getGeo(1) -> getX() - getGeo(0) -> getX()) * (getGeo(1) -> getX() - getGeo(0) -> getX()) 
            +  (getGeo(1) -> getY() - getGeo(0) -> getY()) * (getGeo(1) -> getY() - getGeo(0) -> getY()));  
}
