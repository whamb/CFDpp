#include <stdexcept>

#include <Cell.hpp>

const CellID Cell::nghbrCell(const Face& face) const{
    const auto& nghbrCellIds = face.cellId();
    if(nghbrCellIds[0] != m_id && nghbrCellIds[1] != m_id)
        throw std::runtime_error("Face is not associated to this cell");
    return (nghbrCellIds[0] != m_id ? nghbrCellIds[0] 
                                    : nghbrCellIds[1]);
}