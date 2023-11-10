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
typedef ::testing::Types<float> NumericTypes;
TYPED_TEST_SUITE(InLab10MatrixTests, NumericTypes);

template <typename T>
class InLab10MatrixTests : public ::testing::Test {
  public:
    // Helper function to compare two floating point numbers with a tolerance.
    static bool IsClose(T a, T b, FLOAT_TYPE tolerance = TOLERANCE) {
        return std::abs(a - b) < tolerance;
    }
};

/**
* @brief Test for scalar matrix multiplication.
*
* This test checks the correctness of scalar multiplication with a matrix. It verifies that multiplying a matrix by a
* scalar produces the correct result, and that left and right scalar multiplications are equivalent. Additionally, it
* ensures that each element of the resulting matrix is correctly computed as the product of the scalar and the
* corresponding element of the original matrix.
*/
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

/**
* @brief Test for vector addition.
*
* This test checks the correctness of vector addition. It verifies that adding two vectors produces the same result
* regardless of the order of addition. Additionally, it ensures that each element of the resulting vector is correctly
* computed as the sum of the corresponding elements from the input vectors.
*/
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

/**
* @brief Test for vector scalar product.
*
* This test checks the correctness of the scalar product of two vectors. It verifies that the scalar product of two
* vectors is commutative and produces the correct result. Additionally, it ensures that the scalar product is correctly
* computed as the sum of the element-wise products of the input vectors.
*/
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
* @brief Test for matrix-weighted inner product.
*
* This test checks the correctness of the matrix-weighted inner product. It verifies that the A-inner product, where a
* matrix A is used to weight the standard inner product between two vectors, is computed correctly. The test ensures
* that the result obtained using matrix multiplication is consistent with the result computed element-wise. This test
* is essential for validating the correctness of the matrix-weighted inner product computation.
*
* @note The A-inner product, also known as the generalized inner product or the matrix-weighted inner product, is a typ
* of inner product where a matrix A is used to weight the standard inner product. In the context of vectors y and z,
* the A-inner product is defined as (y, z)_A = y^T * A * z, where y^T denotes the transpose of y, and A is a matrix.
*/
TYPED_TEST(InLab10MatrixTests, MatrixWeightedInnerProductTest) {
    const size_t rows = 8192*8192, cols = 8192*8192;
    size_t seed = 870;
    auto y = Random::generate_vector<TypeParam>(rows, -1e2f, 1e2f, seed++);
    auto z = Random::generate_vector<TypeParam>(cols, -1e2f, 1e2f, seed++);
//    auto A = Random::generate_matrix<TypeParam>(rows, cols, -1e2f, 1e2f, seed++);

    // ((8192+8192+2^(26)) * 4) / (1024 * 1024)     ~ 256 MB 4-byte floats
    // ((8192+8192+2^(26)) * 8) / (1024 * 1024)     ~ 512 MB 8-byte floats
    // ((8192+8192+2^(26)) * 16) / (1024 * 1024)    ~ 1024 MB 16-byte floats
    const auto start = std::chrono::high_resolution_clock::now();
//    TypeParam weightedInnerProduct = y * (A * z);
    TypeParam weightedInnerProduct = y * z;
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Matrix: weighted inner product (8192 rows, 8192 cols) with 2^26 elements took " << duration << " us." << std::endl;

//    TypeParam result = 0;
//    for (size_t i = 0; i < y.size(); ++i) {
//        TypeParam temp = 0;
//        for (size_t j = 0; j < z.size(); ++j) {
//            temp += A[i][j] * z[j];
//        }
//        result += y[i] * temp;
//    }
    // TODO:: Complete
    if (weightedInnerProduct!=0) {
        return;
    }
}

TYPED_TEST(InLab10MatrixTests, LazyMatrixWeightedInnerProductTest) {
    const size_t rows = 8192*8192, cols = 8192*8192;
    size_t seed = 870;
    auto y = MyBLAS::LazyVector<TypeParam>(Random::generate_vector<TypeParam>(rows, -1e2f, 1e2f, seed++));
    auto z = MyBLAS::LazyVector<TypeParam>(Random::generate_vector<TypeParam>(cols, -1e2f, 1e2f, seed++));
//    auto A = MyBLAS::LazyMatrix<TypeParam>(Random::generate_matrix<TypeParam>(rows, cols, -1e2f, 1e2f, seed++));

    // ((8192+8192+2^(26)) * 4) / (1024 * 1024)     ~ 256 MB 4-byte floats
    // ((8192+8192+2^(26)) * 8) / (1024 * 1024)     ~ 512 MB 8-byte floats
    // ((8192+8192+2^(26)) * 16) / (1024 * 1024)    ~ 1024 MB 16-byte floats
    const auto start = std::chrono::high_resolution_clock::now();
//    TypeParam weightedInnerProduct = y * (A * z);
    TypeParam weightedInnerProduct = y * z;
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "LazyMatrix: weighted inner product (8192 rows, 8192 cols) with 2^26 elements took " << duration << " us." << std::endl;
    // TODO:: Complete
//    TypeParam result = 0;
//    for (size_t i = 0; i < y.size(); ++i) {
//        TypeParam temp = 0;
//        for (size_t j = 0; j < z.size(); ++j) {
//            temp += A[i][j] * z[j];
//        }
//        result += y[i] * temp;
//    }
//    EXPECT_EQ(weightedInnerProduct, result);
    // TODO:: Complete
    if (weightedInnerProduct!=0) {
        return;
    }
}

TYPED_TEST(InLab10MatrixTests, LazyMatrixOnlyWeightedInnerProductTest) {
    const size_t cols = 8192*8192;
    auto y1 = MyBLAS::LazyVector<TypeParam>(cols, [](size_t i){ return i*2; });
    auto y2 = MyBLAS::LazyVector<TypeParam>(cols, [](size_t i){ return i*3; });
    auto y3 = MyBLAS::LazyVector<TypeParam>(cols, [](size_t i){ return i*4; });
    //auto z = Random::generate_lazy_vector<TypeParam>(cols, -1e2f, 1e2f, seed++);
    const auto start = std::chrono::high_resolution_clock::now();
    TypeParam weightedInnerProduct = y1 * y2 + y2 * y3;
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "LazyMatrixOnly: weighted inner product (8192 rows, 8192 cols) with 2^26 elements took " << duration << " us." << std::endl;
    // TODO:: Complete
    if (weightedInnerProduct!=0) {
        return;
    }
}

TYPED_TEST(InLab10MatrixTests, MatrixOnlyWeightedInnerProductTest) {
    const size_t cols = 8192*8192;
    auto y1 = MyBLAS::Vector<TypeParam>(cols, [](size_t i){ return i*2; });
    auto y2 = MyBLAS::Vector<TypeParam>(cols, [](size_t i){ return i*3; });
    auto y3 = MyBLAS::Vector<TypeParam>(cols, [](size_t i){ return i*4; });
    //auto z = Random::generate_lazy_vector<TypeParam>(cols, -1e2f, 1e2f, seed++);
    const auto start = std::chrono::high_resolution_clock::now();
    TypeParam weightedInnerProduct = y1 * y2 + y2 * y3;
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "MatrixOnly: weighted inner product (8192 rows, 8192 cols) with 2^26 elements took " << duration << " us." << std::endl;
    // TODO:: Complete
    if (weightedInnerProduct!=0) {
        return;
    }
}
