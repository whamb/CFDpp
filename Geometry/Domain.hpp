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

    Domain(const double lowerBound, const double upperBound, const double dx)
    {
        createNodes(lowerBound, upperBound, dx);
        createFaces();
        createCells();
    }

    void addNode(const Node&& node);
    void addCell(const Cell&& cell);
    void addFace(const Face&& face);

    void createNodes(const double lowerBound, const double upperBound, const double dx);
    void createFaces();
    void createCells();

private:
    
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::vector<std::unique_ptr<Face>> m_faces;
    std::vector<std::unique_ptr<Cell>> m_cells;
};

#endif // DOMAIN_h