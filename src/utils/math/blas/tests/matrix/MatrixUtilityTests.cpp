/**
* @file MatrixUtilityTests.cpp
* @author Arjun Earthperson
* @date 10/27/2023
*
* @brief This file contains unit tests for the utility methods associated with the MyBLAS::Matrix class.
 */

#include "math/blas/BLAS.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

#include <gtest/gtest.h>

#define TOLERANCE 1e-14

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<int, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(MatrixUtilityTests, NumericTypes);

template <typename T>
class MatrixUtilityTests : public ::testing::Test {};

// Test for isSquareMatrix
TYPED_TEST(MatrixUtilityTests, IsSquareMatrixTest) {
    Matrix<TypeParam> squareMatrix(3, 3, 1);
    Matrix<TypeParam> nonSquareMatrix(3, 2, 1);
    EXPECT_TRUE(isSquareMatrix(squareMatrix));
    EXPECT_FALSE(isSquareMatrix(nonSquareMatrix));
}

// Test for isEmptyMatrix
TYPED_TEST(MatrixUtilityTests, IsEmptyMatrixTest) {
    Matrix<TypeParam> emptyMatrix;
    Matrix<TypeParam> nonEmptyMatrix(3, 3, 1);
    EXPECT_TRUE(isEmptyMatrix(emptyMatrix));
    EXPECT_FALSE(isEmptyMatrix(nonEmptyMatrix));
}

// Test for isBinaryMatrix
TYPED_TEST(MatrixUtilityTests, IsBinaryMatrixTest) {
    Matrix<TypeParam> binaryMatrix({{1, 0, 1}, {0, 1, 0}, {1, 0, 1}});
    Matrix<TypeParam> nonBinaryMatrix({{1, 2, 1}, {0, 1, 0}, {1, 0, 1}});
    EXPECT_TRUE(isBinaryMatrix(binaryMatrix));
    EXPECT_FALSE(isBinaryMatrix(nonBinaryMatrix));
}

// Test for isUpperTriangularMatrix
TYPED_TEST(MatrixUtilityTests, IsUpperTriangularMatrixTest) {
    Matrix<TypeParam> upperTriangularMatrix({{1, 2, 3}, {0, 4, 5}, {0, 0, 6}});
    Matrix<TypeParam> nonUpperTriangularMatrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    EXPECT_TRUE(isUpperTriangularMatrix(upperTriangularMatrix));
    EXPECT_FALSE(isUpperTriangularMatrix(nonUpperTriangularMatrix));
}

// Test for isUnitLowerTriangularMatrix
TYPED_TEST(MatrixUtilityTests, IsUnitLowerTriangularMatrixTest) {
    Matrix<TypeParam> unitLowerTriangularMatrix({{1, 0, 0}, {2, 1, 0}, {3, 4, 1}});
    Matrix<TypeParam> nonUnitLowerTriangularMatrix({{1, 0, 0}, {2, 2, 0}, {3, 4, 3}});
    EXPECT_TRUE(isUnitLowerTriangularMatrix(unitLowerTriangularMatrix));
    EXPECT_FALSE(isUnitLowerTriangularMatrix(nonUnitLowerTriangularMatrix));
}

// Test for isPermutationMatrix
TYPED_TEST(MatrixUtilityTests, IsPermutationMatrixTest) {
    Matrix<TypeParam> permutationMatrix({{0, 1, 0}, {1, 0, 0}, {0, 0, 1}});
    Matrix<TypeParam> nonPermutationMatrix({{1, 1, 0}, {0, 1, 0}, {0, 0, 1}});
    EXPECT_TRUE(isPermutationMatrix(permutationMatrix));
    EXPECT_FALSE(isPermutationMatrix(nonPermutationMatrix));
}

// Test for isSymmetricMatrix
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixTest) {
    Matrix<TypeParam> symmetricMatrix({
        {1, 2, 3},
        {2, 4, 5},
        {3, 5, 6}
    });
    Matrix<TypeParam> nonSymmetricMatrix({
        {1, 2, 3},
        {2, 4, 6},
        {3, 5, 9}
    }); // Last element changed to ensure non-symmetry
    EXPECT_TRUE(isSymmetricMatrix(symmetricMatrix));
    EXPECT_FALSE(isSymmetricMatrix(nonSymmetricMatrix)); // This should now pass
}

// Test for noZerosInDiagonal
TYPED_TEST(MatrixUtilityTests, NoZerosInDiagonalTest) {
    Matrix<TypeParam> matrixWithNoZerosInDiagonal({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix<TypeParam> matrixWithZerosInDiagonal({{0, 2, 3}, {4, 0, 6}, {7, 8, 0}});
    EXPECT_TRUE(noZerosInDiagonal(matrixWithNoZerosInDiagonal));
    EXPECT_FALSE(noZerosInDiagonal(matrixWithZerosInDiagonal));
}

// Test for isDiagonallyDominant
TYPED_TEST(MatrixUtilityTests, IsDiagonallyDominantTest) {
    Matrix<TypeParam> diagonallyDominantMatrix({{3, -1, -1}, {-1, 3, -1}, {-1, -1, 3}});
    Matrix<TypeParam> nonDiagonallyDominantMatrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    EXPECT_TRUE(isDiagonallyDominant(diagonallyDominantMatrix));
    EXPECT_FALSE(isDiagonallyDominant(nonDiagonallyDominantMatrix));
}

// Test for haveEqualRank
TYPED_TEST(MatrixUtilityTests, HaveEqualRankTest) {
    Matrix<TypeParam> matrix(3, 3, 1);
    Vector<TypeParam> vector(3, 1);
    Vector<TypeParam> nonEqualRankVector(4, 1);
    EXPECT_TRUE(haveEqualRank(matrix, vector));
    EXPECT_FALSE(haveEqualRank(matrix, nonEqualRankVector));
}

// Test for elementwise Product
TYPED_TEST(MatrixUtilityTests, ElemenwiseProductTest) {
    Matrix<TypeParam> matrixA({{1, 2}, {3, 4}});
    Matrix<TypeParam> matrixB({{5, 6}, {7, 8}});
    Matrix<TypeParam> expectedInnerProduct({{3, 8}, {35, 48}});
    EXPECT_EQ(MyBLAS::Matrix<TypeParam>::elementwiseProduct(matrixA, matrixB), expectedInnerProduct);
}

// Test for matrixWeightedInnerProduct
TYPED_TEST(MatrixUtilityTests, MatrixWeightedInnerProductTest) {
    Vector<TypeParam> vectorY({1, 2});
    Vector<TypeParam> vectorZ({3, 4});
    Matrix<TypeParam> matrixA({{5, 6}, {7, 8}});
    TypeParam expectedWeightedInnerProduct = 145; // Corrected expected result
    EXPECT_EQ(matrixWeightedInnerProduct(vectorY, vectorZ, matrixA), expectedWeightedInnerProduct); // This should now pass
}

// Test for an empty matrix
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixEmptyMatrix) {
    Matrix<TypeParam> emptyMatrix;
    EXPECT_FALSE(MyBLAS::isSymmetricMatrix(emptyMatrix));
}

// Test for a symmetric matrix
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixSymmetric) {
    Matrix<TypeParam> symmetricMatrix({{1, 2, 3}, {2, 4, 5}, {3, 5, 6}});
    EXPECT_TRUE(MyBLAS::isSymmetricMatrix(symmetricMatrix));
}

// Test for a symmetric matrix with negative values
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixSymmetricNegativeValues) {
    MyBLAS::Matrix<TypeParam> symmetricMatrix({{-1, -2, -3}, {-2, -4, -5}, {-3, -5, -6}});
    EXPECT_TRUE(MyBLAS::isSymmetricMatrix(symmetricMatrix));
}

// Test for a non-square matrix
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixNonSquare) {
    MyBLAS::Matrix<TypeParam> nonSquareMatrix({{1, 2, 3}, {4, 5, 6}});
    EXPECT_FALSE(MyBLAS::isSymmetricMatrix(nonSquareMatrix));
}

// Test for a 1x1 matrix (trivially symmetric)
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixOneByOne) {
    MyBLAS::Matrix<TypeParam> oneByOneMatrix({{1}});
    EXPECT_TRUE(MyBLAS::isSymmetricMatrix(oneByOneMatrix));
}

// Test for a matrix with floating-point values
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixFloatingPoint) {
    MyBLAS::Matrix<long double> symmetricMatrix({{1.0, 2.5, 3.5}, {2.5, 4.0, 5.5}, {3.5, 5.5, 6.0}});
    MyBLAS::Matrix<double> symmetricMatrix1({{1.0, 2.5, 3.5}, {2.5, 4.0, 5.5}, {3.5, 5.5, 6.0}});
    MyBLAS::Matrix<float> symmetricMatrix2({{1.0, 2.5, 3.5}, {2.5, 4.0, 5.5}, {3.5, 5.5, 6.0}});
    EXPECT_TRUE(MyBLAS::isSymmetricMatrix(symmetricMatrix));
    EXPECT_TRUE(MyBLAS::isSymmetricMatrix(symmetricMatrix1));
    EXPECT_TRUE(MyBLAS::isSymmetricMatrix(symmetricMatrix2));
}

// Test for a matrix with floating-point values that are not symmetric due to precision
TYPED_TEST(MatrixUtilityTests, IsSymmetricMatrixFloatingPointPrecision) {
    MyBLAS::Matrix<long double> nonSymmetricMatrix({{1.0, 2.5, 3.5}, {2.5, 4.0, 5.5}, {3.5, 5.5000001, 6.0}});
    MyBLAS::Matrix<double> nonSymmetricMatrix1({{1.0, 2.5, 3.5}, {2.5, 4.0, 5.5}, {3.5, 5.5000001, 6.0}});
    MyBLAS::Matrix<double> nonSymmetricMatrix2({{1.0, 2.5, 3.5}, {2.5, 4.0, 5.5}, {3.5, 5.5000001, 6.0}});
    EXPECT_FALSE(MyBLAS::isSymmetricMatrix(nonSymmetricMatrix));
    EXPECT_FALSE(MyBLAS::isSymmetricMatrix(nonSymmetricMatrix1));
    EXPECT_FALSE(MyBLAS::isSymmetricMatrix(nonSymmetricMatrix2));
}

TYPED_TEST(MatrixUtilityTests, IsPositiveDefiniteMatrixTest) {
    // Positive definite matrix (all eigenvalues are positive)
    Matrix<float> positiveDefiniteMatrix({
        {4, 12, -16},
        {12, 37, -43},
        {-16, -43, 98}
    });

    // Not positive definite (negative eigenvalue)
    Matrix<float> nonPositiveDefiniteMatrix({
        {1, 2, 3},
        {2, 0, 5},
        {3, 5, 6}
    });

    // Not positive definite (not symmetric)
    Matrix<float> nonSymmetricMatrix({
        {1, 2, 3},
        {2, 3, 4},
        {3, 4, 5}
    });

    // Not positive definite (zero on diagonal)
    Matrix<float> zeroDiagonalMatrix({
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    });

    // Positive definite matrix (identity matrix)
    Matrix identityMatrix = Matrix<float>::eye(3);

    // Check if the matrices are positive definite
    EXPECT_TRUE(MyBLAS::isPositiveDefiniteMatrix(positiveDefiniteMatrix));
    EXPECT_FALSE(MyBLAS::isPositiveDefiniteMatrix(nonPositiveDefiniteMatrix));
    EXPECT_FALSE(MyBLAS::isPositiveDefiniteMatrix(nonSymmetricMatrix));
    EXPECT_FALSE(MyBLAS::isPositiveDefiniteMatrix(zeroDiagonalMatrix));
    EXPECT_TRUE(MyBLAS::isPositiveDefiniteMatrix(identityMatrix));
}

} // namespace MyBLAS
