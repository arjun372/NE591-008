/**
* @file MatrixPerformanceTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief This file contains performance tests for the MyBLAS::Matrix class.
 */

#include "math/blas/matrix/Matrix.h"
#include <chrono>
#include <cmath>
#include <gtest/gtest.h>

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<short, int, long, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(PerformanceMatrixTests, NumericTypes);

template <typename T>
class PerformanceMatrixTests : public ::testing::Test {};

TYPED_TEST(PerformanceMatrixTests, MatrixMultiplicationPerformanceTest) {
    const size_t BASE = 2, MIN_POWER = 0, MAX_POWER = 9, STEP = 1;
    const auto min = static_cast<size_t>(std::pow(BASE, MIN_POWER));
    const auto max = static_cast<size_t>(std::pow(BASE, MAX_POWER));
    for (auto size = min; size <= max; size *= (BASE * STEP)) {
        Matrix<TypeParam> m1(size, size, 1);
        Matrix<TypeParam> m2(size, size, 2);

        auto start = std::chrono::high_resolution_clock::now();
        Matrix<TypeParam> m3 = m1 * m2;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<long double> elapsed = end - start;
        std::cout<<"["<<sizeof(m2[0][0])<<"]b: ";
        std::cout << "Time taken for matrix multiplication of size " << size << "x" << size << ": " << elapsed.count() << "s\n";
    }
}

} // namespace MyBLAS