#include <vector>

#include <Geo.hpp>

void Geo::setId(const int id)
{
   m_id = id;
}

const int Geo::getId() const
{
    return m_id;
}


/*const int Geo::getNGeos() const
{
    return m_subGeoIds.size();
}*/