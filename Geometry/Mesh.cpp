#include <iterator>
#include <vector>

#include <Cell.hpp>
#include <Face.hpp>
#include <Mesh.hpp>
#include <Node.hpp>

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

    for(int i=0; i<m_faces.size()-1; ++i){
        addCell(*m_faces[i], *m_faces[i+1]);
    }
}

void Mesh::addNode(const double x)
{   
    m_nodes.push_back(std::make_unique<Node>(m_nodes.size(),x));
}

void Mesh::addFace(const Node& node)
{   
    m_faces.push_back(std::make_unique<Face>(m_faces.size(),node));
}

void Mesh::addCell(Face& f1, Face& f2){
    m_cells.push_back(std::make_unique<Cell>(m_cells.size(),f1,f2));
}


