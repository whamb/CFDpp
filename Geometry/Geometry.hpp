#ifndef GEO_H
#define GEO_H

class Geometry
{
public:
    Geometry() = delete;
    Geometry(const int id) : m_id(id){}

    const int getId() const { return m_id; }

protected:
    int m_id = -1; 
};

#endif //GEO_h

