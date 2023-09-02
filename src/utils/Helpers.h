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

const auto default_precision {std::cout.precision()};
constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};

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

// Custom comparator that maintains insertion order
struct InsertionOrderComparator {
    template <typename T> bool operator()(const T& lhs, const T& rhs) const {
        return lhs < rhs; // Maintain the original order
    }
};
