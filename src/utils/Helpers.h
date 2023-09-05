/**
 * @file Helpers.h
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief This file contains helper functions and structures used throughout the program.
*/
#pragma once

#include <iostream>
#include <iomanip>
#include <limits>

#include <boost/program_options.hpp>

/**
 * @brief This function replaces the value of a specified option in a map with a new value.
 *
 * @tparam T The type of the value to be replaced.
 * @param vm A map containing program options and their values.
 * @param opt The option whose value is to be replaced.
 * @param val The new value to replace the old value with.
 *
 * @note This function uses the boost library's any class to allow for type erasure,
 * meaning the function can accept any type for the new value.
 */

template<class T>
void replace(std::map<std::string, boost::program_options::variable_value>& vm, const std::string& opt, const T& val)
{
    vm[opt].value() = boost::any(val);
}

/**
 * @brief This structure defines a range with a minimum and maximum value.
 *
 * @tparam Generic The type of the minimum and maximum values.
 *
 * @note This structure is a template, meaning it can be used with any type.
 */
template <typename Generic> struct Bounds {
    Generic min;
    Generic max;
};

/**
 * @brief This function fills a vector with evenly spaced values between a start and end value.
 *
 * @tparam T The type of the values to be generated.
 * @param result The vector to be filled with the generated values.
 * @param start The start value of the range.
 * @param end The end value of the range.
 * @param count The number of values to be generated.
 *
 * @note This function uses the std::generate function to generate the values.
 */
template <typename T> void fill_linspace(std::vector<T> result, T start, T end, size_t count) {
    result.reserve(count);

    if (count <= 1) {
        result.clear();
        result.push_back(start);
        return;
    }

    T step = (end - start) / static_cast<T>(count - 1);

    std::generate(result.begin(), result.end(), [&start, step]() mutable {
        T value = start;
        start += step;
        return value;
    });
}
