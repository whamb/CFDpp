#include <iterator>
#include <vector>
#include <math.h>

#include "Cell.hpp"
#include "Constants.hpp"
#include "Face.hpp"
#include "Mesh.hpp"
#include "Node.hpp"
#include "Types.hpp"

/**
 * @brief Constructs a structured 1D mesh from given domain bounds and spacing.
 * 
 * Initializes nodes, faces, and cells based on uniform spacing, and assigns
 * geometric data such as centers, volumes, areas, and normals.
 * 
 * @param lowerBound Domain start position
 * @param upperBound Domain end position
 * @param dx Uniform cell spacing
 */
Mesh::Mesh(const Double lowerBound, const Double upperBound, const Double dx) {
    if(dx < 0 || dx > upperBound - lowerBound)
        throw std::runtime_error("dx value is incompatible.");

    Double x = lowerBound;
    while (x < upperBound - 1e-10) {
        addNode(x);
        x += dx;
    }
    addNode(upperBound);

    for (const auto& node : m_nodes) {
        if(std::abs(node->getX() - lowerBound) < 1e-10 || std::abs(node->getX() - upperBound) < 1e-10)
            addInteriorFace(*node);
        else 
            addBoundaryFace(*node);
    }

    // For the moment, for 1D mesh
    for (FaceID i = 1; i < getNFaces() - 2; ++i) {
        addInteriorCell(i, i + 1);
    }
    addBoundaryCell(0,1);
    addBoundaryCell(getNFaces()-2, getNFaces()-1);

    assert(validate() && "Mesh failed consistency checks.");
    assert(m_cells.size() == getNCells()); 
}

/**
 * @brief Adds a node at a given spatial location.
 * 
 * @param x Coordinate of the new node
 */
void Mesh::addNode(const Double x) {
    m_nodes.push_back(std::make_unique<Node>(m_nodes.size(), x));
}

/**
 * @brief Creates a face associated with a node and assigns default geometry.
 * 
 * @param node Node object from which the face originates
 */
void Mesh::addInteriorFace(const Node& node) {
    auto facePtr = std::make_unique<Face>(m_faces.size(), node.getId());
    Face* rawPtr = facePtr.get();
    m_interiorFaces.push_back(std::move(facePtr));
    m_faces.push_back(rawPtr);
    addFaceGeometry(node, *rawPtr);
}

void Mesh::addBoundaryFace(const Node& node) {
    auto facePtr = std::make_unique<Face>(m_faces.size(), node.getId());
    Face* rawPtr = facePtr.get();
    m_boundaryFaces.push_back(std::move(facePtr));
    m_faces.push_back(rawPtr);
    addFaceGeometry(node, *rawPtr);
}

std::vector<const Face*> Mesh::getFaces() const{
    std::vector<const Face*> view;
    for (Face* f : m_faces)
        view.push_back(f);
    return view;
}

void Mesh::addFaceGeometry(const Node& node, const Face& face) {
    m_faceCenter.push_back(node.getX());
    m_faceArea.push_back(defaultArea);
    // Orientation of face normal : lowerId -> higherId
    (face.getCellId()[0] < face.getCellId()[1]) ? m_faceNormal.push_back(defaultNormal)
                                                : m_faceNormal.push_back(-defaultNormal); 
}

/**
 * @brief Creates a cell defined by two adjacent face IDs.
 * 
 * Assigns the cell to both faces, computes geometric center and volume.
 * 
 * @param f1 Left face ID
 * @param f2 Right face ID
 */
void Mesh::addInteriorCell(FaceID f1, FaceID f2) {
    m_faces[f1]->assignCell(m_cells.size());
    m_faces[f2]->assignCell(m_cells.size());
    
    auto cellPtr = std::make_unique<Cell>(m_cells.size(), f1, f2);
    Cell* rawPtr = cellPtr.get();
    m_interiorCells.push_back(std::move(cellPtr));
    m_cells.push_back(rawPtr);

    addCellGeometry(f1,f2);
    
}

void Mesh::addBoundaryCell(FaceID f1, FaceID f2) {
    m_faces[f1]->assignCell(m_cells.size());
    m_faces[f2]->assignCell(m_cells.size());
    
    auto cellPtr = std::make_unique<Cell>(m_cells.size(), f1, f2);
    Cell* rawPtr = cellPtr.get();
    m_boundaryCells.push_back(std::move(cellPtr));
    m_cells.push_back(rawPtr);

    addCellGeometry(f1,f2);
    
}

void Mesh::addCellGeometry(const FaceID f1, const FaceID f2) {
    m_cellCenter.push_back(0.5 * (m_faceCenter[f1] + m_faceCenter[f2]));
    m_cellVolume.push_back(std::abs(m_faceCenter[f1] - m_faceCenter[f2]));
}

std::vector<const Cell*> Mesh::getCells() const {
    std::vector<const Cell*> view;
    for (Cell* c : m_cells)
        view.push_back(c);
    return view;
}

/**
 * @brief Checks the topological and geometric consistency of the mesh.
 * 
 * Verifies face connectivity and that all cells have positive volume.
 * 
 * @return true if the mesh is valid, false otherwise
 */
bool Mesh::validate() const {
    bool valid = true;

    for (size_t i = 0; i < m_faces.size(); ++i) {
        int count = 0;
        if (m_faces[i]->getCellId()[0] != -1) ++count;
        if (m_faces[i]->getCellId()[1] != -1) ++count;

        if (count == 0) {
            std::cerr << "Face " << i << " is orphaned (0 connected cells)\n";
            valid = false;
        }
        if (count > 2) {
            std::cerr << "Face " << i << " has >2 cells (should never happen)\n";
            valid = false;
        }
    }

    for (size_t i = 0; i < m_cellVolume.size(); ++i) {
        if (m_cellVolume[i] <= 0) {
            std::cerr << "Cell " << i << " has non-positive volume\n";
            valid = false;
        }
    }

    return valid;
}