/**
* @file LazyMatrixTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief TODO::DOCUMENT
*/

#include "math/blas/matrix/LazyMatrix.h"

#include <cmath>
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

class LazyMatrixTests : public ::testing::Test {
  protected:
    // Helper function to compare two floating point numbers with a tolerance.
    static bool IsClose(FLOAT_TYPE a, FLOAT_TYPE b, FLOAT_TYPE tolerance = TOLERANCE) {
        return std::abs(a - b) < tolerance;
    }
};

TEST_F(LazyMatrixTests, SizeTest) {
    LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    EXPECT_EQ(m.rows(), 10);
    EXPECT_EQ(m.cols(), 10);
}

TEST_F(LazyMatrixTests, ElementAccessTest) {
    LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j); });
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.cols(); ++j) {
            EXPECT_TRUE(IsClose(m(i, j), static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j)));
        }
    }
}

TEST_F(LazyMatrixTests, AdditionTest) {
    LazyMatrix<FLOAT_TYPE> m1(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    LazyMatrix<FLOAT_TYPE> m2(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j); });
    auto m3 = m1 + m2;
    for (size_t i = 0; i < m3.rows(); ++i) {
        for (size_t j = 0; j < m3.cols(); ++j) {
            EXPECT_TRUE(IsClose(m3(i, j), static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j) + static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j)));
        }
    }
}

TEST_F(LazyMatrixTests, SubtractionTest) {
    LazyMatrix<FLOAT_TYPE> m1(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    LazyMatrix<FLOAT_TYPE> m2(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j); });
    auto m3 = m1 - m2;
    for (size_t i = 0; i < m3.rows(); ++i) {
        for (size_t j = 0; j < m3.cols(); ++j) {
            EXPECT_TRUE(IsClose(m3(i, j), static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j) - static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j)));
        }
    }
}

TEST_F(LazyMatrixTests, MultiplicationTest) {
    LazyMatrix<FLOAT_TYPE> m1(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    LazyMatrix<FLOAT_TYPE> m2(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j); });
    auto m3 = m1 * m2;
    for (size_t i = 0; i < m3.rows(); ++i) {
        for (size_t j = 0; j < m3.cols(); ++j) {
            EXPECT_TRUE(IsClose(m3(i, j), (static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j)) * (static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j))));
        }
    }
}

TEST_F(LazyMatrixTests, EqualityTest) {
    LazyMatrix<FLOAT_TYPE> m1(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    LazyMatrix<FLOAT_TYPE> m2(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    EXPECT_TRUE(m1 == m2);
}

TEST_F(LazyMatrixTests, InequalityTest) {
    LazyMatrix<FLOAT_TYPE> m1(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    LazyMatrix<FLOAT_TYPE> m2(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(j); });
    EXPECT_TRUE(m1 != m2);
}

TEST_F(LazyMatrixTests, ScalarAdditionTest) {
    LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    auto m2 = m + static_cast<FLOAT_TYPE>(10);
    for (size_t i = 0; i < m2.rows(); ++i) {
        for (size_t j = 0; j < m2.cols(); ++j) {
            EXPECT_TRUE(IsClose(m2(i, j), static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j) + static_cast<FLOAT_TYPE>(10)));
        }
    }
}

TEST_F(LazyMatrixTests, ScalarSubtractionTest) {
    LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    auto m2 = m - static_cast<FLOAT_TYPE>(5);
    for (size_t i = 0; i < m2.rows(); ++i) {
        for (size_t j = 0; j < m2.cols(); ++j) {
            EXPECT_TRUE(IsClose(m2(i, j), static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j) - static_cast<FLOAT_TYPE>(5)));
        }
    }
}

TEST_F(LazyMatrixTests, ScalarMultiplicationTest) {
    LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    auto m2 = m * static_cast<FLOAT_TYPE>(2);
    for (size_t i = 0; i < m2.rows(); ++i) {
        for (size_t j = 0; j < m2.cols(); ++j) {
            EXPECT_TRUE(IsClose(m2(i, j), (static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j)) * static_cast<FLOAT_TYPE>(2)));
        }
    }
}

TEST_F(LazyMatrixTests, ScalarDivisionTest) {
    LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    auto m2 = m / static_cast<FLOAT_TYPE>(2);
    for (size_t i = 0; i < m2.rows(); ++i) {
        for (size_t j = 0; j < m2.cols(); ++j) {
            EXPECT_TRUE(IsClose(m2(i, j), (static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j)) / static_cast<FLOAT_TYPE>(2)));
        }
    }
}

TEST_F(LazyMatrixTests, MatrixVectorMultiplicationTest) {
    MyBLAS::Vector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    MyBLAS::LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j); });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        FLOAT_TYPE expected = 0;
        for (size_t j = 0; j < 10; ++j) {
            expected += (static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j)) * static_cast<FLOAT_TYPE>(j);
        }
        EXPECT_TRUE(IsClose(result[i], expected));
    }
}

TEST_F(LazyMatrixTests, MatrixVectorMultiplicationTest_ScalarMultiplication) {
    MyBLAS::Vector<FLOAT_TYPE> v(10, 2.0);
    MyBLAS::LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i + j); });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        FLOAT_TYPE expected = 0;
        for (size_t j = 0; j < 10; ++j) {
            expected += (static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(j)) * 2.0;
        }
        EXPECT_TRUE(IsClose(result[i], expected));
    }
}

TEST_F(LazyMatrixTests, MatrixVectorMultiplicationTest_ZeroVector) {
    MyBLAS::Vector<FLOAT_TYPE> v(10, 0.0);
    MyBLAS::LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return static_cast<FLOAT_TYPE>(i + j); });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_TRUE(IsClose(result[i], 0.0));
    }
}

TEST_F(LazyMatrixTests, MatrixVectorMultiplicationTest_IdentityMatrix) {
    MyBLAS::Vector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    MyBLAS::LazyMatrix<FLOAT_TYPE> m(10, 10, [](size_t i, size_t j) { return i == j ? 1.0 : 0.0; });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_TRUE(IsClose(result[i], static_cast<FLOAT_TYPE>(i)));
    }
}


} //namespace MyBLAS
