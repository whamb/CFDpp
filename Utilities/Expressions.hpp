#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <math.h>
#include <Types.hpp>

/**
 * @file Expressions.hpp
 * @brief Provides small mathematical expression utilities for numerical algorithms.
 * 
 * This header defines reusable inline mathematical helper functions that operate on 
 * scalar values, often used in CFD or linear algebra formulations. These functions are 
 * designed to be simple, inlined for performance, and easy to extend.
 * 
 * Currently includes:
 * - `posMax(Double d)`: Returns `d` if `d > 0`, otherwise returns `0`.
 */

inline Double posMax(Double d) {
    return (d > 0) ? d : 0;
};

#endif // EXPRESSIONS_HPP