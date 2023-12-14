/**
* @file VectorPerformanceTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief This file contains performance tests for the MyBLAS::Vector class.
 */

#include "math/blas/Ops.h"
#include "math/blas/vector/Vector.h"
#include <chrono>
#include <cmath>
#include <gtest/gtest.h>

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<float, double, long double> NumericTypes;
TYPED_TEST_SUITE(PerformanceVectorTests, NumericTypes);

template <typename T>
class PerformanceVectorTests : public ::testing::Test {};

TYPED_TEST(PerformanceVectorTests, VectorAdditionPerformanceTest) {
    const size_t BASE = 2, MIN_POWER = 10, MAX_POWER = 26, STEP = 2;
    const auto min = static_cast<size_t>(std::pow(BASE, MIN_POWER));
    const auto max = static_cast<size_t>(std::pow(BASE, MAX_POWER));
    for (auto size = min; size <= max; size *= (BASE * STEP)) {
        Vector<TypeParam> v1(size, 1);
        Vector<TypeParam> v2(size, 2);

        auto start = std::chrono::high_resolution_clock::now();
        Vector<TypeParam> v3 = v1 + v2;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time taken for vector addition of size " << size << ": " << elapsed.count() << "s\n";
    }
}

TYPED_TEST(PerformanceVectorTests, VectorScalarMultiplicationPerformanceTest) {
    const size_t BASE = 2, MIN_POWER = 10, MAX_POWER = 26, STEP = 2;
    const auto min = static_cast<size_t>(std::pow(BASE, MIN_POWER));
    const auto max = static_cast<size_t>(std::pow(BASE, MAX_POWER));
    for (auto size = min; size <= max; size *= (BASE * STEP)) {
        Vector<TypeParam> v(size, -2);
        auto scalar = static_cast<TypeParam>(-9);

        auto start = std::chrono::high_resolution_clock::now();
        Vector<TypeParam> result = v * scalar;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time taken for vector-scalar multiplication of size " << size << ": " << elapsed.count() << "s\n";
    }
}

TYPED_TEST(PerformanceVectorTests, VectorMultiplicationPerformanceTest) {
    const size_t BASE = 2, MIN_POWER = 10, MAX_POWER = 26, STEP = 2;
    const auto min = static_cast<size_t>(std::pow(BASE, MIN_POWER));
    const auto max = static_cast<size_t>(std::pow(BASE, MAX_POWER));
    for (auto size = min; size <= max; size *= (BASE * STEP)) {
        Vector<TypeParam> v1(size, [&](size_t i){ return static_cast<TypeParam>(static_cast<long>(i << (i % 19))); });
        Vector<TypeParam> v2(size, [&](size_t i){ return static_cast<TypeParam>(static_cast<long>(i << (i % 7))); });

        auto start = std::chrono::high_resolution_clock::now();
        auto result = v1 * v2;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time taken for vector multiplication of size " << size << ": " << elapsed.count() << "s: "<<(result!=0)<<"\n";
    }
}

TYPED_TEST(PerformanceVectorTests, VectorL2NormPerformanceTest) {
    const size_t BASE = 2, MIN_POWER = 10, MAX_POWER = 26, STEP = 2;
    const auto min = static_cast<size_t>(std::pow(BASE, MIN_POWER));
    const auto max = static_cast<size_t>(std::pow(BASE, MAX_POWER));
    for (auto size = min; size <= max; size *= (BASE * STEP)) {
        Vector<TypeParam> v1(size, -32.102f);
        Vector<TypeParam> v2(size, 33.0201f);

        auto start = std::chrono::high_resolution_clock::now();
        auto result = L2(v1, v2);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Time taken for L2 norm of two vectors of size " << size << ": " << elapsed.count() << "s: "<<(result!=0)<<"\n";
    }
}

} // namespace MyBLAS
