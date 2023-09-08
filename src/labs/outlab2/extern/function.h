/**
 * @file function.h
 * @author Arjun Earthperson
 * @date 09/08/2023
 * @brief !MISSING!
 */

#pragma once

#include <vector>
#include <iostream>

[[maybe_unused]] static inline long double user_defined_fx(const long double x) {
    const long double fx = pow(x,4) - pow(x,3) - pow(x, 2) + x + 1;
    return fx;
}

[[maybe_unused]] static void fill_fx(std::vector<long double> &x, std::vector<long double> &fx) {

    fx.clear();
    fx.resize(x.size());

    // Use std::transform with a lambda function to evaluate fx for each x value
    std::transform(x.begin(), x.end(), fx.begin(), [](double x) {
        return user_defined_fx(x);
    });
}
