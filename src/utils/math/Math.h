/**
* @file Math.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief TODO:: DOCUMENT
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

// TODO:: DOCUMENT
template <typename T>
T pow(T base, int exponent) {
    return std::pow(base, exponent);
}

// TODO:: DOCUMENT
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

}  // namespace MyMath


#endif // NE591_008_MATH_H
