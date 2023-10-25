/**
* @file BaseMatrixTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief This file contains unit tests for the MyBLAS::Matrix class.
 */

#include "math/blas/matrix/Matrix.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<int, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(BaseMatrixTests, NumericTypes);

template <typename T>
class BaseMatrixTests : public ::testing::Test {};


TYPED_TEST(BaseMatrixTests, ConstructorTest) {
    Matrix<TypeParam> m1(3, 3, 1);
    EXPECT_EQ(m1.getRows(), 3);
    EXPECT_EQ(m1.getCols(), 3);
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            EXPECT_EQ(m1[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, AdditionTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    Matrix<TypeParam> m3 = m1 + m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_EQ(m3[i][j], 3);
        }
    }
}

TYPED_TEST(BaseMatrixTests, SubtractionTest) {
    Matrix<TypeParam> m1(2, 2, 3);
    Matrix<TypeParam> m2(2, 2, 2);
    Matrix<TypeParam> m3 = m1 - m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_EQ(m3[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, MultiplicationTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    Matrix<TypeParam> m3 = m1 * m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_EQ(m3[i][j], 4);
        }
    }
}

TYPED_TEST(BaseMatrixTests, ScalarMultiplicationTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = m1 * 2;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 4);
        }
    }
}

TYPED_TEST(BaseMatrixTests, EqualityTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 1);
    EXPECT_TRUE(m1 == m2);
}

TYPED_TEST(BaseMatrixTests, InequalityTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    EXPECT_FALSE(m1 == m2);
}

TYPED_TEST(BaseMatrixTests, ZeroSizeMatrixTest) {
    Matrix<TypeParam> m1(0, 0);
    Matrix<TypeParam> m2 = m1 / 2;
    EXPECT_EQ(m2.getCols(), 0);
    EXPECT_EQ(m1.getCols(), 0);
    EXPECT_EQ(m2.getRows(), 0);
    EXPECT_EQ(m1.getRows(), 0);
}

TYPED_TEST(BaseMatrixTests, IdentityMatrixTest) {
    Matrix<TypeParam> m1 = Matrix<TypeParam>::eye(3);
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            if (i == j) {
                EXPECT_EQ(m1[i][j], 1);
            } else {
                EXPECT_EQ(m1[i][j], 0);
            }
        }
    }
}

TYPED_TEST(BaseMatrixTests, MatrixVectorMultiplicationTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Vector<TypeParam> v1(2, 2);
    Vector<TypeParam> v2 = m1 * v1;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 4);
    }
}

TYPED_TEST(BaseMatrixTests, SubMatrixTest) {
    Matrix<TypeParam> m1(3, 3, 1);
    Matrix<TypeParam> m2 = m1.subMatrix(1, 1, 2, 2);
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 2);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, SetSubMatrixTest) {
    Matrix<TypeParam> m1(3, 3, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    m1.setSubMatrix(1, 1, m2);
    Matrix<TypeParam> m3 = m1.subMatrix(1, 1, 2, 2);
    EXPECT_TRUE(m2 == m3);
}

TYPED_TEST(BaseMatrixTests, SwapRowsTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    m1[0][0] = 2;
    m1.swapRows(0, 1);
    EXPECT_EQ(m1[0][0], 1);
    EXPECT_EQ(m1[1][0], 2);
}

TYPED_TEST(BaseMatrixTests, ScalarDivisionTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = m1 / 2;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, ScalarAdditionTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2 = m1 + 1;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 2);
        }
    }
}

TYPED_TEST(BaseMatrixTests, ScalarSubtractionTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = m1 - 1;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, EmptyMatrixTest) {
    Matrix<TypeParam> m1;
    EXPECT_EQ(m1.getRows(), 0);
    EXPECT_EQ(m1.getCols(), 0);
}

TYPED_TEST(BaseMatrixTests, NonSquareMatrixTest) {
    Matrix<TypeParam> m1(2, 3, 1);
    EXPECT_EQ(m1.getRows(), 2);
    EXPECT_EQ(m1.getCols(), 3);
}

//TYPED_TEST(BaseMatrixTests, MatrixMultiplicationDimensionMismatchTest) {
//    Matrix<TypeParam> m1(2, 3, 1);
//    Matrix<TypeParam> m2(2, 2, 1);
//    EXPECT_DEATH(m1 * m2, ".*");
//}
//
//TYPED_TEST(BaseMatrixTests, MatrixVectorMultiplicationDimensionMismatchTest) {
//    Matrix<TypeParam> m1(2, 3, 1);
//    Vector<TypeParam> v1(2, 1);
//    EXPECT_DEATH(m1 * v1, ".*");
//}
//
//TYPED_TEST(BaseMatrixTests, SubMatrixOutOfBoundsTest) {
//    Matrix<TypeParam> m1(2, 2, 1);
//    EXPECT_DEATH(m1.subMatrix(1, 1, 2, 2), ".*");
//}
//
//TYPED_TEST(BaseMatrixTests, SetSubMatrixOutOfBoundsTest) {
//    Matrix<TypeParam> m1(2, 2, 1);
//    Matrix<TypeParam> m2(2, 2, 2);
//    EXPECT_DEATH(m1.setSubMatrix(1, 1, m2), ".*");
//}
//
//TYPED_TEST(BaseMatrixTests, SwapRowsOutOfBoundsTest) {
//    Matrix<TypeParam> m1 = Matrix<TypeParam>::eye(3);
//    EXPECT_DEATH(m1.swapRows(0, 4), ".*");
//}
//
//TYPED_TEST(BaseMatrixTests, ScalarDivisionByZeroTest) {
//    Matrix<TypeParam> m1(2, 2, 2);
//    EXPECT_DEATH(m1 / 0, ".*");
//}
//
//TYPED_TEST(BaseMatrixTests, PushBackRowSizeMismatchTest) {
//    Matrix<TypeParam> m1(2, 2, 1);
//    std::vector<TypeParam> row(3, 1);
//    EXPECT_DEATH(m1.push_back(row), ".*");
//}
//
//TYPED_TEST(BaseMatrixTests, OperatorIndexOutOfBoundsTest) {
//    Matrix<TypeParam> m1(2, 2, 1);
//    EXPECT_THROW(m1[2], std::exception);
//}

} // namespace MyBLAS
