/**
 * @file RootFinder.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief This file contains root finding math functions
 */

#ifndef NE591_008_ROOTFINDER_H
#define NE591_008_ROOTFINDER_H

#include <cmath>
#include <functional>

/**
 * @namespace RootFinder
 * @brief A namespace containing functions for finding roots of mathematical
 * functions
 */
namespace RootFinder {

/**
 * @brief Finds the root of a given function using the Newton-Raphson method
 *
 * The Newton-Raphson method is an iterative method for finding the roots of a
 * real-valued function. The method requires the function f(x) and its
 * derivative f'(x) as inputs, along with an initial guess x0. The method
 * updates the guess using the formula: x1 = x0 - f(x0) / f'(x0), and repeats
 * until the convergence criteria are met.
 *
 * @tparam T The type of the input and output values (e.g., double, float)
 * @param f A std::function representing the function f(x) for which the root is
 * to be found
 * @param df A std::function representing the derivative of the function f'(x)
 * @param x0 The initial guess for the root
 * @param epsilon The convergence criteria (default value: 1e-10)
 * @return The root of the function f(x) found using the Newton-Raphson method
 */
template <typename T>
static T usingNewtonRaphson(const std::function<T(T)> &f,
                            const std::function<T(T)> &df, const T x0,
                            const T epsilon = 1e-10) {
    T x_previous, x = x0;
    do {
        x_previous = x;
        x = x_previous - (f(x_previous) / df(x_previous));
    } while (std::abs(x - x_previous) > epsilon);
    return x;
}
} // namespace RootFinder

#endif // NE591_008_ROOTFINDER_H
