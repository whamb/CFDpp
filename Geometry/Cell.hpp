#ifndef CELL_h
#define CELL_h

#include <array>
#include <math.h>
#include <vector>

#include "Geometry.hpp"
#include "Face.hpp"
#include "Types.hpp"

/**
 * @brief Represents a finite volume cell in 1D space.
 * 
 * A Cell is defined by two face IDs (left and right) and inherits its unique identifier 
 * from the Geometry base class. It provides basic topology access and validity checks 
 * used in mesh traversal and flux computations.
 */
class Cell : public Geometry
{
public:
    Cell() = delete;

    Cell(const CellID id, FaceID f1, FaceID f2) : 
        Geometry(id), m_faceIds({f1, f2}) {
        assert(isValid() && "Invalid Cell \n");
    }

    const std::array<FaceID, 2> getFaceIds() const { return m_faceIds; }

    bool isValid() const {
        return (m_faceIds[0] != -1 && m_faceIds[1] != -1);
    }

    friend std::ostream& operator<<(std::ostream& os, Cell& cell) {
        os << "Cell id =  " << cell.getId() << ": Face0 Id = " << cell.m_faceIds[0]
           << ", Face1 Id = " << cell.m_faceIds[1] << "\n";
        return os;
    }

private:
    std::array<FaceID, 2> m_faceIds { -1, -1 };
};

#endif // CELL_h