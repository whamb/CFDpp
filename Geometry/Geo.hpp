#ifndef GEO_H
#define GEO_H

#include <vector>


class Geo
{
public:
    Geo() : m_id(-1){}
    
    Geo(const int id) : m_id(id){}

    void setId(const int id);

    const int getId() const;    

protected:
    int m_id; 
};

#endif //GEO_h

