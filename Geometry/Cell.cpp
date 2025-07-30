#include <Cell.hpp>

const CellID Cell::getNghbrCell(const Face& face) const{
    const auto nghbrCells = face.getCellId();
    if(nghbrCells[0] != m_id && nghbrCells[1] != m_id)
        throw std::runtime_error("Face is not associated to this cell");
    return (nghbrCells[0] == m_id ? nghbrCells[1] 
                                  : nghbrCells[0]);
}