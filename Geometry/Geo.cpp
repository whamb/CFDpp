#include <vector>

#include<Geo.hpp>

template <typename T>
void Geo<T>::setId(const int id)
{
   m_id = id;
}

template <typename T>
const int Geo<T>::getId() const
{
    return m_id();
}

template <typename T>
const int Geo<T>::getNGeos() const
{
    return m_subGeo.size();
}

template <typename T>
const T* Geo<T>::getGeo(const int i) const
{
   assert("Out of subgeo bounds", i > getNGeos());
   return m_subGeo[i]; 
}

template <typename T>
std::vector<const T *> Geo<T>::addGeo(const T *geo)
{
   m_subGeo.push_back(geo);
   return m_subGeo;
}