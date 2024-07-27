#ifndef DOMAIN_h
#define DOMAIN_h

#include <memory>
#include <vector>
#include <array>

#include <Face.hpp>
#include <Cell.hpp>

class Domain
{
public:
    
    std::vector<std::unique_ptr<Node>> getNodes() const;
    std::vector<std::unique_ptr<Face>> getFaces() const;
    std::vector<std::unique_ptr<Cell>> getCells() const;

    std::vector<std::unique_ptr<Node>> addNode(const Node&& node);
    std::vector<std::unique_ptr<Cell>> addCell(const Cell&& cell);
    std::vector<std::unique_ptr<Face>> addFace(const Face&& face);

private:
    
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Face>> m_faces;
    std::vector<std::unique_ptr<Cell>> m_cells;
};

#endif // DOMAIN_h