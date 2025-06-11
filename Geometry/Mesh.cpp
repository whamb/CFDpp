#include <iterator>
#include <vector>
#include <math.h>

#include "Cell.hpp"
#include "Constants.hpp"
#include "Face.hpp"
#include "Mesh.hpp"
#include "Node.hpp"
#include "Types.hpp"

Mesh::Mesh(const double lowerBound, const double upperBound, const double dx){
    assert(dx > 0 && "dx has to be positive");
    assert(dx <= upperBound-lowerBound && "dx has to be smaller than domain size");

    double x = lowerBound;
    while(x <= upperBound){   
        addNode(x);
        x += dx;
    }    
    if (x < upperBound + dx){
        addNode(upperBound);
    }

    for(const auto& node : m_nodes){
        addFace(*node);
    }

    for(FaceID i=0; i<m_faces.size()-1; ++i){
        addCell(i,i+1);
    }

    assert(validate() && "Mesh failed consistency checks.");
}

void Mesh::addNode(const double x)
{   
    m_nodes.push_back(std::make_unique<Node>(m_nodes.size(),x));
}

void Mesh::addFace(const Node& node)
{   
    m_faces.push_back(std::make_unique<Face>(m_faces.size(),node.getId()));
    m_faceCenter.push_back(node.getX());
    m_faceArea.push_back(defaultArea);
    m_faceNormal.push_back(defaultNormal);
}

void Mesh::addCell(FaceID f1, FaceID f2){
    m_faces[f1]->assignCell(m_cells.size());
    m_faces[f2]->assignCell(m_cells.size());
    m_cells.push_back(std::make_unique<Cell>(m_cells.size(),f1,f2));
    m_cellCenter.push_back(0.5*(m_faceCenter[f1] + m_faceCenter[f2]));
    m_cellVolume.push_back(std::abs(m_faceCenter[f1] - m_faceCenter[f2]));
}

bool Mesh::validate() const {
    bool valid = true;

    for (size_t i = 0; i < m_faces.size(); ++i) {
        int count = 0;
        if (m_faces[i]->getCellId(0) != -1) ++count;
        if (m_faces[i]->getCellId(1) != -1) ++count;

        if (count == 0) {
            std::cerr << "Face " << i << " is orphaned (0 connected cells)\n";
            valid = false;
        }
        if (count > 2) {
            std::cerr << "Face " << i << " has >2 cells (should never happen)\n";
            valid = false;
        }
    }

    for (size_t i = 0; i < m_cellVolume.size(); ++i) {
        if (m_cellVolume[i] <= 0) {
            std::cerr << "Cell " << i << " has non-positive volume\n";
            valid = false;
        }
    }

    return valid;
}


