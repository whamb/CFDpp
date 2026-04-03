#include <cassert>

#include <Cell.hpp>

const CellID Cell::nghbrCell(const Face& face) const{
    const auto& nghbrCellIds = face.cellId();
    assert(nghbrCellIds[0] == m_id || nghbrCellIds[1] == m_id);
    return (nghbrCellIds[0] != m_id ? nghbrCellIds[0] 
                                    : nghbrCellIds[1]);
}