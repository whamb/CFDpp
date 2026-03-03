#include <stdexcept>

#include <Face.hpp>

/**
 * @brief Assigns a cell to the face.
 * 
 * Automatically fills the first available slot (0 or 1) for the face's adjacent cells.
 * Throws an exception if both slots are already occupied.
 * 
 * @param id CellID to assign to the face
 * @throws std::runtime_error if both adjacent cell slots are already filled
 */
void Face::assignCell(CellID id) {
    if(m_cellIds[0] > -1 && m_cellIds[1] > -1)
        throw std::runtime_error("Face already has two cells assigned.");
    if (m_cellIds[0] == -1) {
        m_cellIds[0] = id;
    } 
    else {
        m_cellIds[1] = id;
    }
}