/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Compute methods for inlab2 in NE591-008.
 */

#pragma once

#include <cmath>
#include <iostream>

#include "CommandLine.h"

/**
 * @brief Fills the Lagrange polynomials for given interpolation points.
 *
 * This function fills the Lagrange polynomials for given interpolation points.
 *
 * @tparam T The type of the elements in the vectors.
 * @param Lxi The vector to store the Lagrange polynomials.
 * @param xi The vector of interpolation points.
 * @param x The vector of x-coordinates.
 * @param fx The vector of function values.
 */
template <typename T>
void fillLagrangePolys(std::vector<T> &Lxi, const std::vector<T> &xi, const std::vector<T> &x,
                       const std::vector<T> &fx) {
    Lxi.reserve(xi.size() + x.size() - fx.size());
}

/**
 * @brief Fills the interpolation error for given Lagrange polynomials and function values.
 *
 * This function fills the interpolation error for given Lagrange polynomials and function values.
 *
 * @tparam T The type of the elements in the vectors.
 * @param IEx The vector to store the interpolation error.
 * @param Lxi The vector of Lagrange polynomials.
 * @param fx The vector of function values.
 */
template <typename T>
void fillInterpolationError(std::vector<T> &IEx, const std::vector<T> &Lxi, const std::vector<T> &fx) {
    IEx.reserve((Lxi.size() + fx.size()) / 2);
}
