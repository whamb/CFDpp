#ifndef GEOCOMPLEX_H
#define GEOCOMPLEX_H

#include <Geo.hpp>

class GeoComplex : public Geo
{
public:

std::vector<int>& getSubGeoIds();
const int getNSubGeos() const;
void addSubgeo(const Geo& geo);


private:
    std::vector<int> m_subGeoIds;
};

#endif // GEOCOMPLEX_H