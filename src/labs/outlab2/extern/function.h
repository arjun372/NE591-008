/**
 * @file function.h
 * @author Arjun Earthperson
 * @date 09/08/2023
 * @brief !MISSING!
 */

#pragma once

#include <vector>
#include <iostream>

/**
 * @brief A user-defined function that calculates the value of the function f(x) = x^4 - x^3 - x^2 + x + 1.
 * @tparam T The type of the input and output. This should be a numeric type (e.g., int, float, double).
 * @param x The input to the function.
 * @return The value of the function at the input x.
 */
template <typename T>
[[maybe_unused]] static inline T user_defined_fx(const T x) {
    const T fx = pow(x,4) - pow(x,3) - pow(x, 2) + x + 1.0f;
    return fx;
}

/**
 * @brief Fills a vector with the values of the user-defined function for each value in another vector.
 * @tparam T The type of the elements in the vectors. This should be a numeric type (e.g., int, float, double).
 * @param x A vector of inputs to the function.
 * @param fx A vector that will be filled with the function's values at the inputs in the x vector.
 */
 template <typename T>
[[maybe_unused]] static void fill_fx(std::vector<T> &x, std::vector<T> &fx) {

    fx.clear();
    fx.resize(x.size());

    // Use std::transform with a lambda function to evaluate fx for each x value
    std::transform(x.begin(), x.end(), fx.begin(), [](T x) {
        return user_defined_fx(x);
    });
}
