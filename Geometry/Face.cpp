#include "Face.hpp"

void Face::assignCell(CellID id){
    if (m_cellIds[0] == -1)
        m_cellIds[0] = id;
    else if (m_cellIds[1] == -1)
        m_cellIds[1] = id;
    else
        throw std::runtime_error("Face already has two cells");
}