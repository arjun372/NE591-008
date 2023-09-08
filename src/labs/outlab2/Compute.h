/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Compute methods for inlab2 in NE591-008.
 */

#pragma once

#include <iostream>
#include <cmath>

#include "utils/CommandLine.h"
#include "utils/Stopwatch.h"

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
static void fillLagrangePolys(std::vector<T> &Lxi, const std::vector<T> &xi, const std::vector<T> &x, const std::vector<T> &fx) {
    Lxi.reserve(xi.size());

    const size_t n = std::min(x.size(), fx.size());
    const size_t m = std::min(xi.size(), Lxi.size());

    Stopwatch<Nanoseconds> timer;
    timer.restart();

    for(size_t i = 0; i < xi.size(); i++) {
        T Px = 0.0f;
        for(size_t j = 0; j < n; j++) {
            T Pjx = fx[j];
            for (size_t k = 0; k < n; k++) {
                T Pjxk = (xi[i] - x[k]) / (x[j] - x[k]);
                if (k != j) {
                    Pjx *= Pjxk;
                }
            }
            Px += Pjx;
        }
        Lxi[i] = Px;
    }
    timer.click();
    std::cout << "Computing m=("<<m<<") Lagrange polynomials for n=("<<n<<") took "
              << (static_cast<long double>(timer.duration().count())) << " ns"<<std::endl
              << (static_cast<long double>(timer.duration().count())) << " ns"<<std::endl;
}

/**
 * @brief Fills the interpolation error for given Lagrange polynomials and function values.
 *
 * This function fills the interpolation error for given Lagrange polynomials and function values.
 *
 * @tparam T The type of the elements in the vectors.
 * @param IEx The vector to store the interpolation error.
 * @param Lxi The vector of Lagrange polynomials.
 * @param fxi The vector of function values.
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
    std::transform(Lxi.begin(), Lxi.end(), fxi.begin(), IEx.begin(),
                   [](T x, T y) {
                       return std::abs(x - y);
                   }
    );
}
