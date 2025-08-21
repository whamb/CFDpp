#include <Mesh.hpp>
#include <TripletSystem.hpp>
#include <Types.hpp>

TripletSystem::TripletSystem(const Mesh& mesh){
    m_rhs.resize(mesh.getNCells());
}

void TripletSystem::addToLHS(const CellID row, const CellID column, const Double value) {
    //Deal with the case the i,j index already created
    auto key = std::make_pair(row,column);
    auto found = m_indexMap.find(key);

    if(found == m_indexMap.end()){
        m_value.push_back(value);
        m_row.push_back(row);
        m_column.push_back(column);
        m_indexMap[key] = m_value.size() - 1;
    }
    else{
        m_value[found -> second] += value;
    }
}

void TripletSystem::clear(){
    m_value.clear();
    m_row.clear();
    m_column.clear();
    m_rhs.assign(m_rhs.size(), 0.0);
    m_indexMap.clear();
}