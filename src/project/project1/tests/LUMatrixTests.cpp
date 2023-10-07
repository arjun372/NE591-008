/**
 * @file LUMatrixTests.cpp
 * @brief MISSING!
 */

#include "math/blas/MyBLAS.h"
#include "math/factorization/LU.h"
#include <gtest/gtest.h>

class FactorizeValidLUMatrixTest : public ::testing::TestWithParam<MyBLAS::Matrix<long double>> {
  protected:
    MyBLAS::Matrix<long double> coefficients;
};

TEST_P(FactorizeValidLUMatrixTest, FactorizesValidLowerMatrix) {
    MyBLAS::Matrix<long double> A, L, U;
    A = GetParam();
    L = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    U = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    MyBLAS::LU::factorize<long double>(L, U, A);
    EXPECT_TRUE(MyBLAS::isUnitLowerTriangularMatrix(L)) << "L: " << std::endl << L << std::endl;
}

TEST_P(FactorizeValidLUMatrixTest, FactorizesValidUpperMatrix) {
    MyBLAS::Matrix<long double> A, L, U;
    A = GetParam();
    L = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    U = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    MyBLAS::LU::factorize<long double>(L, U, A);
    EXPECT_TRUE(MyBLAS::isUpperTriangularMatrix(U)) << "U: " << std::endl << U << std::endl;
}

INSTANTIATE_TEST_SUITE_P(
    InputValues, FactorizeValidLUMatrixTest,
    ::testing::Values(MyBLAS::Matrix<long double>{{1.0, 1.0, 2.0}, {2.0, 4.0, 7.0}, {3.0, 7.0, 15.0}},
                      MyBLAS::Matrix<long double>{
                          {2.0, 7.0, 1.0, 4.0},
                          {3.0, -2.0, 0.0, 3.0},
                          {1.0, 5.0, 3.0, 5.0},
                          {1.0, 2.0, 3.0, 2.0},
                      },
                      MyBLAS::Matrix<long double>{{2.0, 7.0, 1.0}, {3.0, -2.0, 0.0}, {1.0, 5.0, 3.0}},
                      MyBLAS::Matrix<long double>{
                          {1.0, 2.0, 3.0, 4.0}, {2.0, 3.0, 4.0, 1.0}, {3.0, 4.0, 1.0, 2.0}, {4.0, 1.0, 2.0, 3.0}}));

class FactorizeInvalidLUMatrixTest : public ::testing::TestWithParam<MyBLAS::Matrix<long double>> {
  protected:
    MyBLAS::Matrix<long double> coefficients;
    MyBLAS::Matrix<long double> L;
    MyBLAS::Matrix<long double> U;
};

TEST_P(FactorizeInvalidLUMatrixTest, FactorizesInvalidLowerMatrix) {
    MyBLAS::Matrix<long double> A, L, U;
    A = GetParam();
    L = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    U = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    MyBLAS::LU::factorize<long double>(L, U, A);
    EXPECT_FALSE(MyBLAS::isUnitLowerTriangularMatrix(L)) << "L: " << std::endl << L << std::endl;
}

TEST_P(FactorizeInvalidLUMatrixTest, FactorizesInvalidUpperMatrix) {
    MyBLAS::Matrix<long double> A, L, U;
    A = GetParam();
    L = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    U = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
    MyBLAS::LU::factorize<long double>(L, U, A);
    EXPECT_FALSE(MyBLAS::isUpperTriangularMatrix(U)) << "U: " << std::endl << U << std::endl;
}

INSTANTIATE_TEST_SUITE_P(
    InputValues, FactorizeInvalidLUMatrixTest,
    ::testing::Values(MyBLAS::Matrix<long double>{{1.0, 2.0, 3.0, 4.0, 5.0},
                                                  {2.0, 3.0, 4.0, 5.0, 1.0},
                                                  {3.0, 4.0, 5.0, 1.0, 2.0},
                                                  {4.0, 5.0, 1.0, 2.0, 3.0},
                                                  {5.0, 1.0, 2.0, 3.0, 4.0}},
                      MyBLAS::Matrix<long double>{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},
                      MyBLAS::Matrix<long double>{{1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}));
