#include <iterator>
#include <vector>

#include <Cell.hpp>
#include <Domain.hpp>
#include <Face.hpp>
#include <Mesh.hpp>
#include <Node.hpp>


void Mesh::createNodes(Domain& domain)
{
    double x = m_lowerBound;
    while(x <= m_upperBound){   
        domain.addNode(Node(x));
        x += m_dx;
    }
    
    if (x < m_upperBound + m_dx){
        domain.addNode(Node(m_upperBound));
    }  
}

void Mesh::createFaces(Domain& domain)
{
    for(const auto& node : domain.getNodes()){
        domain.addFace(Face(node.get(),node.get()));        // for the moment, nodes are collapsed
    }
}

void Mesh::createCells(Domain& domain)
{
    for (size_t i = 0; i < domain.getFaces().size() - 1; ++i) {
        const auto& face1 = (domain.getFaces())[i];
        const auto& face2 = (domain.getFaces())[i+1]; // Check if there is a next element
        domain.addCell(Cell(face1.get(),face2.get()));
    }
}

