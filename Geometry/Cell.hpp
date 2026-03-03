#ifndef CELL_h
#define CELL_h

#include <array>
#include <math.h>
#include <vector>

#include <Face.hpp>
#include <Types.hpp>

/**
 * @brief Represents a finite volume cell in 1D space.
 * 
 * A Cell is defined by two face IDs (left and right) and inherits its unique identifier 
 * from the Geometry base class. It provides basic topology access and validity checks 
 * used in mesh traversal and flux computations.
 */
class Cell
{
public:

    Cell() : m_id(-1), m_faceIds({-1,-1})
    {}
    Cell(const CellID id, FaceID f1, FaceID f2) : m_id(id), m_faceIds({f1, f2}) {
        assert(isValid() && "Invalid Cell \n");}

    CellID id() const { return m_id; }

    const std::array<FaceID, 2> faceIds() const { return m_faceIds; }

    bool isValid() const {return (m_faceIds[0] != -1 || m_faceIds[1] != -1);}

    bool isBoundary() const {return (m_faceIds[0] == -1 || m_faceIds[0] == -1) ? true 
                                                                        : false;}
    const CellID nghbrCell(const Face& face) const;

private:
    CellID m_id;
    std::array<FaceID, 2> m_faceIds {-1, -1};
};

#endif // CELL_h