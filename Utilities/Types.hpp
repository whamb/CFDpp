#ifndef TYPES_HPP
#define TYPES_HPP

/**
 * @file Types.hpp
 * @brief Defines fundamental type aliases used throughout the solver.
 * 
 * Provides semantic clarity by assigning explicit names to index types 
 * (e.g., NodeID, CellID) and value types (e.g., Double) used in the 
 * finite volume and linear algebra modules.
 */

using NodeID = int;   ///< Identifier for mesh nodes
using FaceID = int;   ///< Identifier for mesh faces
using CellID = int;   ///< Identifier for mesh cells

using Double = double; ///< Precision type used for scalar field values

#endif // TYPES_HPP