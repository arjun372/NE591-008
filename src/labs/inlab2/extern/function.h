/**
 * @file function.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief This file contains the declaration of the function `fill_fx`.
 *        The function is used to fill a vector with values based on another
 * vector.
 */

#pragma once

#include <iostream>
#include <vector>

/**
 * @brief Fill the `fx` vector, optionally using the values in the `x` vector.
 * @param x The input vector containing `x` values.
 * @param fx The output vector to be filled with `fx` values.
 * @details This function takes an input vector `x` and an output vector `fx`.
 *          It fills the `fx` vector with values.
 *          The `fx` vector is assumed to be initially zeroed out.
 *          The function does not return any value.
 * @note The `x` and `fx` vectors must have the same size.
 * @note The `fx` vector will be modified by this function.
 * @note The `x` vector will not be modified by this function.
 */
[[maybe_unused]] void fill_fx(const std::vector<long double> &x,
                              std::vector<long double> &fx) {

    if (x.size() != fx.size()) {
        std::cerr << "Warning: Vectors are of different lengths: size(x): "
                  << x.size() << ", size(f[x]): " << fx.size() << "\n";
    }

    //    fx.push_back(5);
    //    fx.push_back(5);
    //    fx.push_back(5);
    //    fx.push_back(5);

    //
    //    // trivial
    //    for(size_t i = 0; i < x.size(); i++) {
    //        fx[i] = 2.0f * x[i];
    //    }
}
