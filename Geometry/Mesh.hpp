#ifndef MESH_H
#define MESH_H

#include <cassert>
#include <span>
#include <vector>
#include <memory>

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
    const std::vector<std::unique_ptr<Node>>& nodes() const { return m_nodes; }
    
    //**************************************//
    //********** Face Generation ********** //
    //**************************************//
    void addInteriorFace(const Node& node);
    void addBoundaryFace(const Node& node);

    const std::vector<Face*>&                 interiorFaces()   const { return m_interiorFaces; }
    const std::vector<Face*>&                 boundaryFaces()   const { return m_boundaryFaces; }
    const std::vector<std::unique_ptr<Face>>& faces()           const { return m_faces; }

    //Special version for llvm
    //std::vector<const Face*> getFaces() const;

    FaceID nInteriorFaces() const {return m_interiorFaces.size();}
    FaceID nBoundaryFaces() const {return m_boundaryFaces.size();}
    FaceID nFaces()         const {return m_faces.size();}

    const std::vector<Double>& faceCenter() const {return m_faceCenter;}
    const std::vector<Double>& faceArea()   const {return m_faceArea;}
    const std::vector<Double>& faceNormal() const {return m_faceNormal;}

    void addFaceGeometry(const Node& node, const Face& face);
    void addFaceNormal  (const Face& face);
    
    //**************************************//
    //********** Cell generation ***********//
    //**************************************//
    void addInteriorCell(FaceID f1, FaceID f2);
    void addBoundaryCell(FaceID f1, FaceID f2);

    const std::vector<Cell*>&                 interiorCells() const { return m_interiorCells; }
    const std::vector<Cell*>&                 boundaryCells() const { return m_boundaryCells; }
    const std::vector<std::unique_ptr<Cell>>& cells()         const { return m_cells; }

    //Special version for llvm
    //std::vector<const Cell*> getCells() const;

    FaceID nInteriorCells() const {return m_interiorCells.size();}
    FaceID nBoundaryCells() const {return m_boundaryCells.size();}
    FaceID nCells()         const {return m_cells.size();}

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
    std::vector<Face*> m_interiorFaces;
    std::vector<Face*> m_boundaryFaces;
    std::vector<std::unique_ptr<Face>> m_faces;
    std::vector<Double> m_faceCenter;
    std::vector<Double> m_faceArea;
    std::vector<Double> m_faceNormal;

    //***********************************//
    //********** Cell relative **********//
    //***********************************//
    std::vector<Cell*> m_interiorCells;
    std::vector<Cell*> m_boundaryCells;
    std::vector<std::unique_ptr<Cell>> m_cells;
    std::vector<Double> m_cellCenter;
    std::vector<Double> m_cellVolume;

};

#endif // MESH_H