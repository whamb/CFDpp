#ifndef FACE_h
#define FACE_h

#include <assert.h>
#include <array>

#include <Geometry.hpp>
#include <Node.hpp>
#include "Types.hpp"

/**
 * @brief Represents a face in a 1D finite volume mesh.
 * 
 * A Face connects a node and up to two adjacent cells (left/right).
 * It stores topological information used for flux calculations and mesh connectivity.
 * Inherits a unique ID from the Geometry base class.
 */
class Face : public Geometry
{
public:
    Face() = delete;

    Face(const FaceID id, const NodeID nodeId)
        : Geometry(id), m_nodeId(nodeId) {
        assert(isValid() && "Invalid Face \n");
    }

    const CellID getCellId(const int index) const {
        assert(index < 2);
        return m_cellIds[index];
    }

    void setCell(int index, CellID cellId) {
        assert(index < 2);
        m_cellIds[index] = cellId;
    }

    bool isValid() const {
        return (m_nodeId >= 0);
    }

    void assignCell(CellID id);

    friend std::ostream& operator<<(std::ostream& os, Face& face) {
        os << "Face id =  " << face.getId()
           << ": Node Id = " << face.m_nodeId << "\n";
        return os;
    }

private:
    NodeID m_nodeId;
    std::array<CellID, 2> m_cellIds { -1, -1 };
};

#endif // FACE_h