#ifndef MESH_H
#define MESH_H

#include <cassert>
#include <vector>

#include <Cell.hpp>
#include <Face.hpp>
#include <Node.hpp>
class Mesh
{

public:

Mesh(const Double lowerBound, const Double upperBound, const Double dx);

const std::vector<std::unique_ptr<Node>>& getNodes() const { return m_nodes; }
const std::vector<std::unique_ptr<Face>>& getFaces() const { return m_faces; }
const std::vector<std::unique_ptr<Cell>>& getCells() const { return m_cells; }

void addNode(const Double x);
void addFace(const Node& node);
void addCell(FaceID f1, FaceID f2);
bool validate() const;

private:

std::vector<std::unique_ptr<Node> > m_nodes;
std::vector<std::unique_ptr<Face> > m_faces;
std::vector<std::unique_ptr<Cell> > m_cells;

std::vector<Double> m_cellCenter;
std::vector<Double> m_cellVolume;
std::vector<Double> m_faceCenter;
std::vector<Double> m_faceArea;
std::vector<Double> m_faceNormal;
};

#endif