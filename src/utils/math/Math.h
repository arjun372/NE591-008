/**
* @file Math.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief This file contains the declaration of various mathematical functions.
* @details The functions declared in this file include checking if a numeric value is finite,
* calculating the power of a base number, and calculating the absolute value of a number.
* These functions are declared within the MyMath namespace. Specializations for the __float128
* type are also provided for the isfinite and pow functions.
 */

#ifndef NE591_008_MATH_H
#define NE591_008_MATH_H

#include <limits>
#include <cmath>
#include <quadmath.h>

namespace MyMath {

/**
 * @brief Check if a numeric value is finite.
 *
 * This function checks whether a given numeric value is finite. For most numeric types,
 * it delegates to the standard library's `std::isfinite` function. However, it has a specialization
 * for the `__float128` type to handle its specific finiteness criteria.
 *
 * @tparam T The type of the numeric value to check for finiteness.
 * @param value The numeric value to check.
 * @return true if the value is finite, false otherwise.
 */
template <typename T>
bool isfinite(T value) {
    return std::isfinite(value);
}

/**
 * @brief Specialization for the __float128 type.
 *
 * This specialization of the `isfinite` function is specifically designed to handle
 * the `__float128` type. It checks whether the given `__float128` value is within the range
 * of finite values for this type.
 *
 * @param value The __float128 value to check for finiteness.
 * @return true if the value is finite, false otherwise.
 */
template <>
bool isfinite(__float128 value) {
    // Check if value is within the range of finite values for __float128
    return (value > -__float128(INFINITY) && value < __float128(INFINITY));
}

/**
 * @brief A template function to calculate the power of a base number.
 * @details This function uses the std::pow function to calculate the power of a base number.
 * @tparam T The type of the base number.
 * @param base The base number.
 * @param exponent The exponent to which the base number is raised.
 * @return The result of the base number raised to the exponent.
 */
template <typename T>
T pow(T base, int exponent) {
    return std::pow(base, exponent);
}

/**
 * @brief A template specialization function to calculate the power of a __float128 base number.
 * @details This function uses a custom algorithm to calculate the power of a __float128 base number.
 * @param base The base number.
 * @param exponent The exponent to which the base number is raised.
 * @return The result of the base number raised to the exponent.
 */
template <>
__float128 pow(__float128 base, int exponent) {
    if (exponent == 0)
        return 1.0;
    if (exponent == 1)
        return base;

    __float128 result = 1.0;

    bool isNegativeExponent = exponent < 0;
    exponent = isNegativeExponent ? -exponent : exponent;
    __float128 current = base;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= current;
        }
        current = current * current;
        exponent /= 2;
    }

    return isNegativeExponent ? 1.0 / result : result;
}

/**
 * @brief A template function to calculate the absolute value of a number.
 * @tparam T The type of the number.
 * @param x The number whose absolute value is to be calculated.
 * @return The absolute value of the number.
 */
template <typename T>
T abs(T x) {
    return x < 0 ? -x : x;
}

}  // namespace MyMath


#endif // NE591_008_MATH_H
