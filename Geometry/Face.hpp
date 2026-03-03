#ifndef FACE_h
#define FACE_h

#include <assert.h>
#include <array>

#include <Node.hpp>
#include <Types.hpp>

/**
 * @brief Represents a face in a 1D finite volume mesh.
 * 
 * A Face connects a node and up to two adjacent cells (left/right).
 * It stores topological information used for flux calculations and mesh connectivity.
 * Inherits a unique ID from the Geometry base class.
 */
class Face
{
public:
    Face() : m_id(-1), m_nodeId(-1), m_cellIds({-1,-1})
    {}
    Face(const FaceID id, const NodeID nodeId) : m_id(id), m_nodeId(nodeId), m_cellIds({-1,-1}) {
        assert(isValid() && "Invalid Face \n"); 
    }

    FaceID id() const { return m_id; }
    void  assignCell(CellID id);

    const std::array<CellID, 2> cellId() const {return m_cellIds;}

    bool  isValid() const {return (m_nodeId >= 0);}

    bool  isBoundary() const {return (m_cellIds[0] == -1 || m_cellIds[0] == -1) ? true
                                                                                : false;}

private:
    FaceID m_id;
    NodeID m_nodeId;
    std::array<CellID, 2> m_cellIds;
};

#endif // FACE_h