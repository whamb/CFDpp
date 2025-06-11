#ifndef MESH_H
#define MESH_H

#include <cassert>
#include <vector>

#include <Cell.hpp>
#include <Domain.hpp>
#include <Face.hpp>
#include <Node.hpp>
class Mesh
{

public:

// Build a 1D mesh
Mesh(const double lowerBound, const double upperBound, const double dx);

const std::vector<std::unique_ptr<Node>>& getNodes() const { return m_nodes; }
const std::vector<std::unique_ptr<Face>>& getFaces() const { return m_faces; }
const std::vector<std::unique_ptr<Cell>>& getCells() const { return m_cells; }

void addNode(const double x);
void addFace(const Node& node);
void addCell(Face& f1, Face& f2);

private:

std::vector<std::unique_ptr<Node> > m_nodes;
std::vector<std::unique_ptr<Face> > m_faces;
std::vector<std::unique_ptr<Cell> > m_cells;

};

#endif