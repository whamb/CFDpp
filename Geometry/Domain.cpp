#include <memory>
#include <vector>
#include <array>
#include <cassert>

#include <Cell.hpp>
#include <Domain.hpp>
#include <Face.hpp>

std::vector<std::unique_ptr<Node>> Domain::getNodes() const
{
    return m_nodes;
}

std::vector<std::unique_ptr<Face>> Domain::getFaces() const
{
    return m_faces;
}

std::vector<std::unique_ptr<Cell>> Domain::getCells() const
{
    return m_cells;
}

std::vector<std::unique_ptr<Node>> Domain::addNode(const Node&& node)
{
    m_nodes.push_back(std::make_unique<Node>(node));
    return m_nodes;
}

std::vector<std::unique_ptr<Face>> Domain::addFace(const Face&& face)
{
    assert(("Invalid node", !face.isValid()));
    m_faces.push_back(std::make_unique<Face>(face));
    return m_faces;
}

std::vector<std::unique_ptr<Cell>> Domain::addCell(const Cell&& cell)
{
    assert(("Invalid node", !cell.isValid()));
    m_cells.push_back(std::make_unique<Cell>(cell));
    return m_cells;
}
