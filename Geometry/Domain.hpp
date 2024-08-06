#ifndef DOMAIN_h
#define DOMAIN_h

#include <memory>
#include <vector>
#include <array>

#include <Face.hpp>
#include <Cell.hpp>
#include <Node.hpp>

class Domain
{
public:

    Domain(const double lowerBound, const double upperBound, const double dx);

    const int getNNodes();
    const int getNFaces();
    const int getNCells();

    std::vector<std::unique_ptr<Node> >& getNodes();
    std::vector<std::unique_ptr<Face> >& getFaces();
    std::vector<std::unique_ptr<Cell> >& getCells();

    void addNode(Node&& node);
    void addCell(Cell&& cell);
    void addFace(Face&& face);

    void createNodes(const double lowerBound, const double upperBound, const double dx);
    void createFaces();
    void createCells();

private:
    
    std::vector<std::unique_ptr<Node> > m_nodes;
    std::vector<std::unique_ptr<Face> > m_faces;
    std::vector<std::unique_ptr<Cell> > m_cells;
};

#endif // DOMAIN_h