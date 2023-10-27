/**
* @file BaseMatrixTests.cpp
* @author Arjun Earthperson
* @date 10/27/2023
*
* @brief This file contains unit tests for the Matrix-Vector operations needed in InLab10
 */

#include "math/Random.h"
#include "math/blas/matrix/LazyMatrix.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

// Define a list of numeric types to run the tests with
typedef ::testing::Types<float, double, long double> NumericTypes;
TYPED_TEST_SUITE(InLab10MatrixTests, NumericTypes);

template <typename T>
class InLab10MatrixTests : public ::testing::Test {
  public:
    // Helper function to compare two floating point numbers with a tolerance.
    static bool IsClose(T a, T b, FLOAT_TYPE tolerance = TOLERANCE) {
        return std::abs(a - b) < tolerance;
    }
};

TYPED_TEST(InLab10MatrixTests, ScalarMatrixMultiplicationTest) {
    TypeParam scalar = 1.20382e-18f;
    const size_t rows = 31, cols = 67;
    size_t seed = 3172;
    auto matrix = Random::generate_matrix<TypeParam>(rows, cols, -1e-4f, 1e4f, seed++);
    EXPECT_EQ(matrix.getRows(), 31);
    EXPECT_EQ(matrix.getCols(), 67);
    auto leftScaled = scalar * matrix;
    auto rightScaled = matrix * scalar;
    EXPECT_EQ(leftScaled, rightScaled);
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(leftScaled[i][j], scalar * matrix[i][j]);
            EXPECT_EQ(leftScaled[i][j], rightScaled[i][j]);
        }
    }
}

TYPED_TEST(InLab10MatrixTests, VectorSumTest) {
    const size_t size = 31;
    size_t seed = 3372;
    auto vector1 = Random::generate_vector<TypeParam>(size, -1e-20f, 1e20f, seed++);
    auto vector2 = Random::generate_vector<TypeParam>(size, -1e-20f, 1e20f, seed++);
    EXPECT_TRUE((vector1 + vector2) == (vector2 + vector1));
    for (size_t i = 0; i < vector1.size(); ++i) {
        EXPECT_EQ(vector1[i] + vector2[i], vector2[i] + vector1[i]);
    }
}


TYPED_TEST(InLab10MatrixTests, VectorScalarProductTest) {
    const size_t size = 31;
    size_t seed = 372;
    auto vector1 = Random::generate_vector<TypeParam>(size, -1e-20f, 1e20f, seed++);
    auto vector2 = Random::generate_vector<TypeParam>(size, -1e-20f, 1e20f, seed++);
    EXPECT_TRUE((vector1 * vector2) == (vector2 * vector1));

    TypeParam result = 0;
    for (size_t i = 0; i < vector1.size(); i++) {
        result += vector1[i] * vector2[i];
    }
    EXPECT_EQ(vector1 * vector2, result);
}

/**
 * The A-inner product, also known as the generalized inner product or the matrix-weighted inner product, is a type of
 * inner product where a matrix A is used to weight the standard inner product. In the context of vectors y and z,
 * the A-inner product is defined as (y, z)_A = y^T * A * z, where y^T denotes the transpose of y, and A is a matrix.
 */
TYPED_TEST(InLab10MatrixTests, MatrixWeightedInnerProductTest) {
    const size_t rows = 31, cols = 47;
    size_t seed = 3072;
    auto y = MyBLAS::LazyVector<TypeParam>(Random::generate_vector<TypeParam>(rows, -1e-2f, 1e2f, seed++));
    auto z = MyBLAS::LazyVector<TypeParam>(Random::generate_vector<TypeParam>(cols, -1e-2f, 1e2f, seed++));
    auto A = MyBLAS::LazyMatrix<TypeParam>(Random::generate_matrix<TypeParam>(rows, cols, -1e-2f, 1e2f, seed++));

    MyBLAS::LazyVector<TypeParam> vectorAz = A * z;
    TypeParam weightedInnerProduct = y * vectorAz;

    TypeParam result = 0;
    for (size_t i = 0; i < y.size(); ++i) {
        TypeParam temp = 0;
        for (size_t j = 0; j < z.size(); ++j) {
            temp += A[i][j] * z[j];
        }
        result += y[i] * temp;
    }

    EXPECT_EQ(weightedInnerProduct, result);
}
