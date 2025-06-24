#include <TripletSystem.hpp>

void TripletSystem::add(Double value, CellID row, CellID column){
    ++m_size;
    m_value.push_back(value);
    m_row.push_back(row);
    m_column.push_back(column);
}