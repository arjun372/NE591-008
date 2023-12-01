/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/08/2023
 * @brief This file contains the declaration of two template functions, fillLagrangePolys and fillInterpolationError,
 *        which are used for computing Lagrange polynomials and interpolation errors respectively.
 */

#pragma once

#include <cmath>
#include <iostream>

#include "CommandLine.h"
#include "utils/Stopwatch.h"

/**
 * @brief This function computes the Lagrange polynomials for a given set of data points.
 * @tparam T The data type of the elements in the vectors.
 * @param Lxi A vector to store the computed Lagrange polynomials.
 * @param xi A vector containing the x-coordinates of the interpolation points.
 * @param x A vector containing the x-coordinates of the data points.
 * @param fx A vector containing the y-coordinates (function values) of the data points.
 * @note The function also prints the time taken to compute the Lagrange polynomials.
 */
template <typename T>
static void fillLagrangePolys(std::vector<T> &Lxi, const std::vector<T> &xi, const std::vector<T> &x,
                              const std::vector<T> &fx) {

    const size_t n = std::min(x.size(), fx.size());
    const size_t m = std::min(xi.size(), Lxi.size());
    Lxi.reserve(m);

    Stopwatch<Nanoseconds> timer;
    timer.restart();

    for (size_t i = 0; i < xi.size(); i++) {
        T Px = 0.0f;
        for (size_t j = 0; j < n; j++) {
            T Pjx = fx[j];

            // go from k -> j-1
            T Pjx_product1 = 1.0f;
            for (size_t k = 0; k < j; k++) {
                T Pjxk = (xi[i] - x[k]) / (x[j] - x[k]);
                Pjx_product1 *= Pjxk;
            }

            // skip k == j;

            // then, from from k+1 -> n
            T Pjx_product2 = 1.0f;
            for (size_t k = j + 1; k < n; k++) {
                T Pjxk = (xi[i] - x[k]) / (x[j] - x[k]);
                Pjx_product2 *= Pjxk;
            }
            Px += (Pjx * Pjx_product1 * Pjx_product2);
        }
        Lxi[i] = Px;
    }

    timer.click();
    std::cout << "Computing m=(" << m << ") Lagrange polynomials for n=(" << n << ") took "
              << (static_cast<long double>(timer.duration().count())) << " ns" << std::endl;
}

/**
 * @brief This function computes the interpolation error for a given set of data points and their corresponding Lagrange
 * polynomials.
 * @tparam T The data type of the elements in the vectors.
 * @param IEx A vector to store the computed interpolation errors.
 * @param Lxi A vector containing the computed Lagrange polynomials.
 * @param fxi A vector containing the function values at the interpolation points.
 * @note The function will print an error message and return if the sizes of Lxi and fxi are not equal.
 */
template <typename T>
static void fillInterpolationError(std::vector<T> &IEx, const std::vector<T> &Lxi, const std::vector<T> &fxi) {
    // Check if the input vectors have the same size
    if (Lxi.size() != fxi.size()) {
        std::cerr << "Error: L(xi), f(xi) must have the same size." << std::endl;
        return;
    }

    IEx.clear();
    IEx.resize(Lxi.size());

    // Use std::transform with a lambda function to calculate absolute errors
    std::transform(Lxi.begin(), Lxi.end(), fxi.begin(), IEx.begin(), [](T x, T y) { return std::abs(x - y); });
}
