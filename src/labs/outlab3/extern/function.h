/**
 * @file function.h
 * @author Arjun Earthperson
 * @date 09/08/2023
 * @brief This file contains the declaration of a user-defined function and a function to fill a vector with the values
 * of the user-defined function for each value in another vector.
 */

#pragma once

#include <vector>
#include <iostream>

/**
 * @brief A user-defined function that calculates the value of the function f(x) = e^(x)
 * @tparam T The type of the input and output. This should be a numeric type (e.g., int, float, double).
 * @param x The input to the function.
 * @return The value of the function at the input x.
 */
template <typename T>
[[maybe_unused]] static inline T user_defined_fx(const T x) {
    const T fx = std::exp(x);
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
