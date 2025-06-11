#include <memory>
#include <vector>
#include <array>
#include <cassert>
#include <string>

#include <Cell.hpp>
#include <Domain.hpp>
#include <Face.hpp>
#include <Node.hpp>

Domain::Domain(const double lowerBound, const double upperBound, const double dx)
{
    createNodes(lowerBound, upperBound, dx);
    createFaces();
    createCells();
}

const int Domain::getNNodes()
{
    return m_nodes.size();
}

const int Domain::getNFaces()
{
    return m_faces.size();
}

const int Domain::getNCells()
{
    return m_cells.size();
}

std::vector<std::unique_ptr<Node> >& Domain::getNodes()
{
    return m_nodes;
}

std::vector<std::unique_ptr<Face> >& Domain::getFaces()
{
    return m_faces;
}

std::vector<std::unique_ptr<Cell> >& Domain::getCells()
{
    return m_cells;
}

void Domain::addNode(Node&& node)
{   
    node.setId(m_nodes.size());
    m_nodes.push_back(std::make_unique<Node>(node));
}

void Domain::addFace(Face&& face)
{   
    face.setId(m_faces.size());
    if (!face.isValid()) {
        throw std::runtime_error("Invalid node in face" + std::to_string(face.getId()));
    }   
    m_faces.push_back(std::make_unique<Face>(face));
}

void Domain::addCell(Cell&& cell)
{   
    cell.setId(m_cells.size());
    if(!cell.isValid()){
        throw std::runtime_error("Invalid face in cell" + std::to_string(cell.getId()));
    }
    m_cells.push_back(std::make_unique<Cell>(cell));
}

void Domain::createNodes(const double lowerBound, const double upperBound, const double dx)
{   
      
}

void Domain::createFaces()
{
    for(const auto& node : m_nodes){
        addFace(Face(*node,*node));        // for the moment, nodes are collapsed
    }
}

void Domain::createCells()
{
    for (size_t i = 0; i < m_faces.size() - 1; ++i) {
        const auto& face1 = m_faces[i];
        const auto& face2 = m_faces[i+1]; // Check if there is a next element
        addCell(Cell(*face1,*face2));
    }
}
