#ifndef MESH_H
#define MESH_H

#include <cassert>
#include <span>
#include <vector>

#include <Cell.hpp>
#include <Face.hpp>
#include <Node.hpp>

/**
 * @brief Represents a structured 1D finite volume mesh.
 * 
 * Manages mesh topology and geometry, including nodes, faces, and cells.
 * Responsible for building connectivity between elements and storing
 * geometric properties like centers, volumes, areas, and normals.
 */
class Mesh
{
public:
Mesh(const Double lowerBound, const Double upperBound, const Double dx);

//**************************************//
//********** Node generation ********** //
//**************************************//
void addNode(const Double x);
const std::vector<std::unique_ptr<Node>>& getNodes() const { return m_nodes; }

//**************************************//
//********** Face Generation ********** //
//**************************************//
void addInteriorFace(const Node& node);
void addBoundaryFace(const Node& node);
const std::vector<std::unique_ptr<Face>>& getInteriorFaces() const { return m_interiorFaces; }
const std::vector<std::unique_ptr<Face>>& getBoundaryFaces() const { return m_boundaryFaces; }
std::span<Face*> getFaces() { return { m_faces.data(), m_faces.size() }; }
//Special version for llvm
std::vector<const Face*> getFaces() const;
const FaceID getNInteriorFaces() const {return m_interiorFaces.size();}
const FaceID getNBoundaryFaces() const {return m_boundaryFaces.size();}
const FaceID getNFaces() const {return m_faces.size();}
const std::vector<Double>& getFaceCenter() const {return m_faceCenter;}
const std::vector<Double>& getFaceArea()   const {return m_faceArea;}
const std::vector<Double>& getFaceNormal() const {return m_faceNormal;}
void addFaceGeometry(const Node& node, const Face& face);

//**************************************//
//********** Cell generation ***********//
//**************************************//
void addInteriorCell(FaceID f1, FaceID f2);
void addBoundaryCell(FaceID f1, FaceID f2);
const std::vector<std::unique_ptr<Cell>>& getInteriorCells() const { return m_interiorCells; }
const std::vector<std::unique_ptr<Cell>>& getBoundaryCells() const { return m_boundaryCells; }
std::span<Cell*> getCells() { return { m_cells.data(), m_cells.size() }; }
//Special version for llvm
std::vector<const Cell*> getCells() const;
const FaceID getNInteriorCells() const {return m_interiorCells.size();}
const FaceID getNBoundaryCells() const {return m_boundaryCells.size();}
const FaceID getNCells() const {return m_cells.size();}
const std::vector<Double>& getCellCenter() const {return m_cellCenter;}
const std::vector<Double>& getCellVolume() const {return m_cellVolume;}
void addCellGeometry(const FaceID f1, const FaceID f2);

bool validate() const;

private:
//***********************************//
//********** Node relative **********//
//***********************************//
std::vector<std::unique_ptr<Node>> m_nodes;

//***********************************//
//********** Face relative **********//
//***********************************//
std::vector<std::unique_ptr<Face>> m_interiorFaces;
std::vector<std::unique_ptr<Face>> m_boundaryFaces;
std::vector<Face*> m_faces;
std::vector<Double> m_faceCenter;
std::vector<Double> m_faceArea;
std::vector<Double> m_faceNormal;

//***********************************//
//********** Cell relative **********//
//***********************************//
std::vector<std::unique_ptr<Cell>> m_interiorCells;
std::vector<std::unique_ptr<Cell>> m_boundaryCells;
std::vector<Cell*> m_cells;
std::vector<Double> m_cellCenter;
std::vector<Double> m_cellVolume;

};

#endif // MESH_H