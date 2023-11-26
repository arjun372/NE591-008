/**
* @file MatrixMemoryTrackingTests.cpp
* @author Arjun Earthperson
* @date 10/20/2023
*
* @brief This file contains unit tests for testing the allocated memory tracking in the MyBLAS::Matrix class.
 */

#include "math/blas/matrix/Matrix.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<int, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(MatrixMemoryTrackingTests, NumericTypes);

template <typename T>
class MatrixMemoryTrackingTests : public ::testing::Test {
  protected:
    // Helper function to calculate the expected memory usage of a matrix
    static size_t calculateExpectedMemoryUsage(size_t rows, size_t cols) {
        return rows * cols * sizeof(T);
    }
};

TYPED_TEST(MatrixMemoryTrackingTests, ConstructorTest) {
    // Test the memory usage reported by the Matrix class after construction
    const size_t rows = 10;
    const size_t cols = 5;
    const size_t expectedMemoryUsage = this->calculateExpectedMemoryUsage(rows, cols);

    // Reset memory usage before the test
    Matrix<TypeParam>::resetMemoryUsage();

    // Create a matrix and check memory usage
    Matrix<TypeParam> matrix(rows, cols);
    EXPECT_EQ(Matrix<TypeParam>::getCurrentMemoryUsage(), expectedMemoryUsage);

    // Create another matrix and check cumulative memory usage
    Matrix<TypeParam> anotherMatrix(rows, cols);
    EXPECT_EQ(Matrix<TypeParam>::getCurrentMemoryUsage(), 2 * expectedMemoryUsage);

    // After matrices go out of scope, memory usage should be back to zero
}

TYPED_TEST(MatrixMemoryTrackingTests, DestructorTest) {
    const size_t rows = 10;
    const size_t cols = 5;
    const size_t expectedMemoryUsage = this->calculateExpectedMemoryUsage(rows, cols);

    // Reset memory usage before the test
    Matrix<TypeParam>::resetMemoryUsage();

    {
        Matrix<TypeParam> matrix(rows, cols);
        EXPECT_EQ(Matrix<TypeParam>::getCurrentMemoryUsage(), expectedMemoryUsage);
    }

    // After matrix goes out of scope, memory usage should be back to zero
    EXPECT_EQ(Matrix<TypeParam>::getCurrentMemoryUsage(), 0);
}

} // namespace MyBLAS
