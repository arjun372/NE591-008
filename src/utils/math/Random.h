/**
 * @file Random.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the Random namespace which provides functions for generating random vectors and matrices.
*/

#ifndef NE591_008_RANDOM_H
#define NE591_008_RANDOM_H

#include <vector>
#include <random>
#include <algorithm>

#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"

/**
 * @namespace Random
 * @brief This namespace provides functions for generating random vectors and matrices.
 */
namespace Random {

    /**
     * @brief Generates a vector of random numbers.
     * @tparam T The type of the elements in the vector.
     * @param n The size of the vector.
     * @param min The minimum value for the random numbers.
     * @param max The maximum value for the random numbers.
     * @param seed The seed for the random number generator.
     * @return A vector of random numbers.
     */
    template <typename T>
    MyBLAS::Vector<T> generate(const size_t n, T min = 0, T max = 1, const long long seed = 372) {
        if (max < min) {
            auto temp = max;
            max = min;
            min = temp;
        }
        std::random_device rd;
        std::mt19937 stream(seed);
        std::uniform_real_distribution<T> uniform(min, max);
        MyBLAS::Vector<T> samples(n);
        for(size_t idx = 0; idx < n; idx++) {
            samples[idx] = uniform(stream);
        }
        return samples;
    }

    /**
     * @brief Generates a matrix of random numbers.
     * @tparam T The type of the elements in the matrix.
     * @param rows The number of rows in the matrix.
     * @param cols The number of columns in the matrix.
     * @param min The minimum value for the random numbers.
     * @param max The maximum value for the random numbers.
     * @param seed The seed for the random number generator.
     * @return A matrix of random numbers.
     */
    template <typename T>
    MyBLAS::Matrix<T> generate(const size_t rows, const size_t cols, T min = 0, T max = 1, const long long seed = 372) {
        if (max < min) {
            auto temp = max;
            max = min;
            min = temp;
        }
        std::random_device rd;
        std::mt19937 stream(seed);
        std::uniform_real_distribution<T> uniform(min, max);
        MyBLAS::Matrix<T> samples(rows, cols);
        for(size_t row = 0; row < rows; row++) {
            for(size_t col = 0; col < cols; col++) {
                samples[row][col] = uniform(stream);
            }
        }

        return samples;
    }

    /**
     * @brief Generates a binary matrix.
     * @tparam T The type of the elements in the matrix.
     * @param rows The number of rows in the matrix.
     * @param cols The number of columns in the matrix.
     * @param seed The seed for the random number generator.
     * @return A binary matrix.
     */
    template <typename T>
    MyBLAS::Matrix<T> binary(const size_t rows, const size_t cols, const long long seed = 372) {
        std::random_device rd;
        std::mt19937 stream(seed);
        std::uniform_real_distribution<T> uniform(static_cast<T>(0), static_cast<T>(1));
        MyBLAS::Matrix<T> samples(rows, cols);
        for(size_t row = 0; row < rows; row++) {
            for(size_t col = 0; col < cols; col++) {
                const auto sample = uniform(stream);
                samples[row][col] = (1 - sample) < (sample - 0) ? 1 : 0;
            }
        }
        return samples;
    }

}
#endif //NE591_008_RANDOM_H
