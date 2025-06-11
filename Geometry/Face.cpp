#include <iostream>
#include <Face.hpp>

void Face::addCell(const int cellId){
    if(m_cellIds[0] == -1) 
        m_cellIds[0] = cellId;
    else if(m_cellIds[1] == -1) 
        m_cellIds[1] = cellId;
    else
        std::cerr << "Face " << m_id << " already assigned 2 cells";
}