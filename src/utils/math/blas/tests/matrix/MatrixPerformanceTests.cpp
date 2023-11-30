/**
* @file MatrixPerformanceTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief This file contains performance tests for the MyBLAS::Matrix class.
 */

#include "math/Random.h"
#include "math/blas/matrix/Matrix.h"
#include <chrono>
#include <cmath>
#include <gtest/gtest.h>
#include <omp.h>

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<float, double, long double> NumericTypes;
TYPED_TEST_SUITE(PerformanceMatrixTests, NumericTypes);

template <typename T>
class PerformanceMatrixTests : public ::testing::Test {};

TYPED_TEST(PerformanceMatrixTests, MatrixMultiplicationPerformanceTest) {
    size_t seed = 372;
    const size_t BASE = 2, MIN_POWER = 0, MAX_POWER = 10, STEP = 1;
    const auto min = static_cast<size_t>(std::pow(BASE, MIN_POWER));
    const auto max = static_cast<size_t>(std::pow(BASE, MAX_POWER));
    for (auto size = min; size <= max; size *= (BASE * STEP)) {

        auto m1 = Random::generate_matrix<TypeParam>(size, size, -100, 100, ++seed);
        auto m2 = Random::generate_matrix<TypeParam>(size, size, -20, 20, ++seed);

        auto start = std::chrono::high_resolution_clock::now();
        Matrix<TypeParam> m3 = m1 * m2;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<long double> elapsed = end - start;
        std::cout<<"["<<sizeof(m2[0][0])<<"]b: ";
        std::cout << "Time taken for matrix multiplication of size " << size << "x" << size << ": " << elapsed.count() << "s\n";

        Matrix<TypeParam> m4(size, size, 0);
        start = std::chrono::high_resolution_clock::now();
        #pragma omp parallel for default(none) shared(m4, m2, m1, size)
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                for (size_t k = 0; k < size; ++k) {
                    m4[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Time taken for openmp matrix multiplication of size " << size << "x" << size << ": " << elapsed.count() << "s\n";

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                ASSERT_NEAR(static_cast<double>(m4[i][j]), static_cast<double>(m3[i][j]), static_cast<double>(1e-6));
            }
        }
    }
}

TYPED_TEST(PerformanceMatrixTests, MatrixAdditionPerformanceTest) {
    size_t seed = 281;
    const size_t BASE = 2, MIN_POWER = 0, MAX_POWER = 14, STEP = 1;
    const auto min = static_cast<size_t>(std::pow(BASE, MIN_POWER));
    const auto max = static_cast<size_t>(std::pow(BASE, MAX_POWER));
    for (auto size = min; size <= max; size *= (BASE * STEP)) {

        auto m1 = Random::generate_matrix<TypeParam>(size, size, -100, 100, ++seed);
        auto m2 = Random::generate_matrix<TypeParam>(size, size, -20, 20, ++seed);

        auto start = std::chrono::high_resolution_clock::now();
        Matrix<TypeParam> m3 = m1 + m2;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<long double> elapsed = end - start;
        std::cout<<"["<<sizeof(m2[0][0])<<"]b: ";
        std::cout << "Time taken for matrix addition of size " << size << "x" << size << ": " << elapsed.count() << "s\n";

        Matrix<TypeParam> m4(size, size, 0);
        start = std::chrono::high_resolution_clock::now();
        #pragma omp parallel for default(none) shared(m4, m2, m1, size)
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                m4[i][j] = m1[i][j] + m2[i][j];
            }
        }
        end = std::chrono::high_resolution_clock::now();
        elapsed = end - start;
        std::cout << "Time taken for openmp matrix addition of size " << size << "x" << size << ": " << elapsed.count() << "s\n";

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                ASSERT_NEAR(static_cast<double>(m4[i][j]), static_cast<double>(m3[i][j]), static_cast<double>(1e-6));
            }
        }
    }
}


//TYPED_TEST(PerformanceMatrixTests, VectorAdditionGPUOffloadTest) {
//    const size_t vector_size = 1000000; // Size of the vectors
////    auto a = float[1000];
//    std::vector<TypeParam> a(vector_size, 1); // Initialize vector 'a' with 1s
//    std::vector<TypeParam> b(vector_size, 2); // Initialize vector 'b' with 2s
//    std::vector<TypeParam> c(vector_size, 0); // Initialize result vector 'c' with 0s
//
//    TypeParam* a_data = a.data();
//    TypeParam* b_data = b.data();
//    TypeParam* c_data = c.data();
//// Offload the vector addition to the GPU
//    #pragma omp target teams distribute parallel for map(to: a_data[0:vector_size], b_data[0:vector_size]) map(from: c_data[0:vector_size])
//        for (size_t i = 0; i < vector_size; ++i) {
//            c_data[i] = a_data[i] + b_data[i];
//        }
//
//        // Check the results on the CPU
//        for (size_t i = 0; i < vector_size; ++i) {
//            ASSERT_EQ(c[i], a[i] + b[i]) << "Vectors a and b were not added correctly on the GPU at index " << i;
//        }
//}

TYPED_TEST(PerformanceMatrixTests, VectorAdditionOpenMPCPUTest) {
        const size_t vector_size = 1000000; // Size of the vectors
        std::vector<float> a(vector_size, 1.0f); // Initialize vector 'a' with 1s
        std::vector<float> b(vector_size, 2.0f); // Initialize vector 'b' with 2s
        std::vector<float> c(vector_size, 0.0f); // Initialize result vector 'c' with 0s

        // Perform the vector addition using OpenMP for parallelization on the CPU
        #pragma omp parallel for
        for (size_t i = 0; i < vector_size; ++i) {
            c[i] = a[i] + b[i];
        }

        // Check the results
        for (size_t i = 0; i < vector_size; ++i) {
            ASSERT_EQ(c[i], a[i] + b[i]) << "Vectors a and b were not added correctly at index " << i;
        }
}
} // namespace MyBLAS
