#ifndef GEO_H
#define GEO_H

/**
 * @brief Base class for geometric entities with unique identifiers.
 * 
 * Provides a common interface for mesh components like cells, faces, and nodes
 * by encapsulating a unique ID. Not intended to be instantiated directly.
 */
class Geometry
{
public:
    Geometry() = delete;
    Geometry(const int id) : m_id(id) {}

    const int getId() const { return m_id; }

protected:
    int m_id = -1;
};

#endif // GEO_H