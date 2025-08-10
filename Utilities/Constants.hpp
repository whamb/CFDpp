#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <Types.hpp>

/**
 * @file Constants.hpp
 * @brief Defines global physical or geometric constants used across the solver.
 * 
 * These constants typically represent default face area, normal orientation, or
 * any commonly used scalar parameters in FVM calculations.
 */

inline constexpr Double defaultArea = 1.0;    ///< Default face area (used in 1D FVM)
inline constexpr Double defaultNormal = 1.0;  ///< Default face normal orientation (+1.0)

inline constexpr int maxIts = 100;  ///< Default face normal orientation (+1.0)

#endif // CONSTANTS_HPP