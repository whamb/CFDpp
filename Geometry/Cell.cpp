#include <Cell.hpp>

const CellID Cell::getNghbrCell(const Face& face) const{
    const auto& nghbrCellIds = face.getCellId();
    /*std::cout << "CellId = " << m_id << "\n";
    std::cout << "FaceId0 = " << this->getFaceIds()[0] << "\n";
    std::cout << "FaceId1 = " << this->getFaceIds()[1] << "\n";
    std::cout << "FaceId = " << face.getId() << "\n";
    std::cout << "nghbrCell[0] = " << nghbrCells[0] << "\n";
    std::cout << "nghbrCell[1] = " << nghbrCells[1] << "\n";*/
    if(nghbrCellIds[0] != m_id && nghbrCellIds[1] != m_id)
        throw std::runtime_error("Face is not associated to this cell");
    return (nghbrCellIds[0] != m_id ? nghbrCellIds[0] 
                                    : nghbrCellIds[1]);
}