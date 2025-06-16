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
    assert(dx > 0 && "dx has to be positive");
    assert(dx <= upperBound - lowerBound && "dx has to be smaller than domain size");

    Double x = lowerBound;
    while (x < upperBound - 1e-10) {
        addNode(x);
        x += dx;
    }
    addNode(upperBound);


    for (const auto& node : m_nodes) {
        addFace(*node);
    }

    for (FaceID i = 0; i < m_faces.size() - 1; ++i) {
        addCell(i, i + 1);
    }

    assert(validate() && "Mesh failed consistency checks.");
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
void Mesh::addFace(const Node& node) {
    m_faces.push_back(std::make_unique<Face>(m_faces.size(), node.getId()));
    m_faceCenter.push_back(node.getX());
    m_faceArea.push_back(defaultArea);
    m_faceNormal.push_back(defaultNormal);
}

/**
 * @brief Creates a cell defined by two adjacent face IDs.
 * 
 * Assigns the cell to both faces, computes geometric center and volume.
 * 
 * @param f1 Left face ID
 * @param f2 Right face ID
 */
void Mesh::addCell(FaceID f1, FaceID f2) {
    m_faces[f1]->assignCell(m_cells.size());
    m_faces[f2]->assignCell(m_cells.size());
    m_cells.push_back(std::make_unique<Cell>(m_cells.size(), f1, f2));
    m_cellCenter.push_back(0.5 * (m_faceCenter[f1] + m_faceCenter[f2]));
    m_cellVolume.push_back(std::abs(m_faceCenter[f1] - m_faceCenter[f2]));
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
        if (m_faces[i]->getCellId(0) != -1) ++count;
        if (m_faces[i]->getCellId(1) != -1) ++count;

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