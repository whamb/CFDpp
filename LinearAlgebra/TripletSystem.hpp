#include <span>
#include <vector>

#include <Mesh.hpp>
#include <Types.hpp>

class TripletSystem
{
public:
TripletSystem(Mesh& mesh){m_rhs.resize(mesh.getNCells());}
void add(Double value, CellID row, CellID column);

private:
CellID m_size = 0;
std::vector<Double> m_value;
std::vector<CellID> m_row;
std::vector<CellID> m_column;
std::vector<Double> m_rhs;

};
