#include <Geo.hpp>
#include <GeoComplex.hpp>

std::vector<int>& GeoComplex::getSubGeoIds()
{
    return m_subGeoIds;
}

const int GeoComplex::getNSubGeos() const
{
    return m_subGeoIds.size();
}

void GeoComplex::addSubgeo(const Geo& geo)
{
    m_subGeoIds.push_back(geo.getId());
}