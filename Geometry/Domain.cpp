#include <memory>
#include <vector>
#include <array>
#include <cassert>

#include <Cell.hpp>
#include <Domain.hpp>
#include <Face.hpp>
#include <Node.hpp>

void Domain::addNode(const Node&& node)
{
    m_nodes.push_back(std::make_unique<Node>(node));
}

void Domain::addFace(const Face&& face)
{
    assert(("Invalid node", !face.isValid()));
    m_faces.push_back(std::make_unique<Face>(face));
}

void Domain::addCell(const Cell&& cell)
{
    assert(("Invalid node", !cell.isValid()));
    m_cells.push_back(std::make_unique<Cell>(cell));
}

void Domain::createNodes(const double lowerBound, const double upperBound, const double dx)
{
    double x = lowerBound;
    while(x <= upperBound){   
        addNode(Node(x));
        x += dx;
    }
    
    if (x < upperBound + dx){
        addNode(Node(upperBound));
    }  
}

void Domain::createFaces()
{
    for(const auto& node : m_nodes){
        addFace(Face(node.get(),node.get()));        // for the moment, nodes are collapsed
    }
}

void Domain::createCells()
{
    for (size_t i = 0; i < m_faces.size() - 1; ++i) {
        const auto& face1 = m_faces[i];
        const auto& face2 = m_faces[i+1]; // Check if there is a next element
        addCell(Cell(face1.get(),face2.get()));
    }
}
