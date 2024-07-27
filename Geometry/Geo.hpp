#ifndef GEO_H
#define GEO_H

#include <vector>

template <typename T>
class Geo
{
public:
    Geo() : m_id(-1){}
    
    Geo(const int id, const std::vector<T>& geos) : m_id(id), m_subGeo(geos){}

    ~Geo() = default;

    void setId(const int id);

    const int getId() const;

    const int getNGeos() const;

    const T* getGeo(int i) const;

    std::vector<const T*> addGeo(const T *geo);

protected:
    std::vector<const T*> m_subGeo;
    int m_id;
    
};

#endif //GEO_h

