/**
* @file LazyMatrixTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*/

#include "math/blas/matrix/LazyMatrix.h"

#include <cmath>
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<int, long, float, double, long double> FloatTypes;
TYPED_TEST_SUITE(LazyMatrixTests, FloatTypes);

template <typename T>
class LazyMatrixTests : public ::testing::Test {
  public:
    // Helper function to compare two floating point numbers with a tolerance.
    static bool IsClose(T a, T b, FLOAT_TYPE tolerance = TOLERANCE) {
        return std::abs(a - b) < tolerance;
    }
};

TYPED_TEST(LazyMatrixTests, SizeTest) {
    LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    EXPECT_EQ(m.getRows(), 10);
    EXPECT_EQ(m.getCols(), 10);
}

TYPED_TEST(LazyMatrixTests, ElementAccessTest) {
    LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) * static_cast<TypeParam>(j); });
    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m(i, j), static_cast<TypeParam>(i) * static_cast<TypeParam>(j)));
        }
    }
}

TYPED_TEST(LazyMatrixTests, AdditionTest) {
    LazyMatrix<TypeParam> m1(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    LazyMatrix<TypeParam> m2(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) * static_cast<TypeParam>(j); });
    LazyMatrix<TypeParam> m3 = m1 + m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m3(i, j), static_cast<TypeParam>(i) + static_cast<TypeParam>(j) + static_cast<TypeParam>(i) * static_cast<TypeParam>(j)));
        }
    }
}

TYPED_TEST(LazyMatrixTests, SubtractionTest) {
    LazyMatrix<TypeParam> m1(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    LazyMatrix<TypeParam> m2(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) * static_cast<TypeParam>(j); });
    auto m3 = m1 - m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m3(i, j), static_cast<TypeParam>(i) + static_cast<TypeParam>(j) - static_cast<TypeParam>(i) * static_cast<TypeParam>(j)));
        }
    }
}

TYPED_TEST(LazyMatrixTests, MatrixMultiplicationTest) {
    LazyMatrix<TypeParam> m1(10, 5, [](size_t i, size_t j) { return static_cast<TypeParam>(i + j); });
    LazyMatrix<TypeParam> m2(5, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i * j); });
    auto m3 = m1 * m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            TypeParam expected = 0;
            for (size_t k = 0; k < m1.getCols(); ++k) {
                expected += m1(i, k) * m2(k, j);
            }
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m3(i, j), expected));
        }
    }
}

TYPED_TEST(LazyMatrixTests, BLASMatrixAdditionTest) {
    LazyMatrix<TypeParam> m1(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    MyBLAS::Matrix<TypeParam> m2(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) * static_cast<TypeParam>(j); });
    auto m12 = m1 + m2;
    auto m21 = m2 + m1;
    EXPECT_EQ(m12.getCols(), m21.getCols());
    EXPECT_EQ(m12.getRows(), m21.getRows());
    for (size_t i = 0; i < m12.getRows(); ++i) {
        for (size_t j = 0; j < m12.getCols(); ++j) {
            const auto actual = (static_cast<TypeParam>(i) + static_cast<TypeParam>(j)) + (static_cast<TypeParam>(i) * static_cast<TypeParam>(j));
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m12(i, j), actual));
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m21(i, j), actual));
        }
    }
}

TYPED_TEST(LazyMatrixTests, BLASMatrixSubtractionTest) {
    LazyMatrix<TypeParam> m1(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    MyBLAS::Matrix<TypeParam> m2(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) * static_cast<TypeParam>(j); });
    auto m12 = m1 - m2;
    auto m21 = m2 - m1;
    for (size_t i = 0; i < m12.getRows(); ++i) {
        for (size_t j = 0; j < m12.getCols(); ++j) {
            const auto actual = (static_cast<TypeParam>(i) + static_cast<TypeParam>(j)) - (static_cast<TypeParam>(i) * static_cast<TypeParam>(j));
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m12(i, j), actual));
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m21(i, j), -actual));
            // Note the negative sign here, as m2 - m1 is not the same as m1 - m2
        }
    }
}

TYPED_TEST(LazyMatrixTests, EqualityTest) {
    LazyMatrix<TypeParam> m1(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    LazyMatrix<TypeParam> m2(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    EXPECT_TRUE(allElementsEqual(m2, m1));
}

TYPED_TEST(LazyMatrixTests, InequalityTest) {
    LazyMatrix<TypeParam> m1(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    LazyMatrix<TypeParam> m2(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) * static_cast<TypeParam>(j); });
    EXPECT_TRUE(!allElementsEqual(m1, m2));
}

TYPED_TEST(LazyMatrixTests, ScalarAdditionTest) {
    LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    auto m2 = m + static_cast<TypeParam>(10);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m2(i, j), static_cast<TypeParam>(i) + static_cast<TypeParam>(j) + static_cast<TypeParam>(10)));
        }
    }
}

TYPED_TEST(LazyMatrixTests, ScalarSubtractionTest) {
    LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    auto m2 = m - static_cast<TypeParam>(5);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m2(i, j), static_cast<TypeParam>(i) + static_cast<TypeParam>(j) - static_cast<TypeParam>(5)));
        }
    }
}

TYPED_TEST(LazyMatrixTests, ScalarMultiplicationTest) {
    LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    auto m2 = m * static_cast<TypeParam>(2);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m2(i, j), (static_cast<TypeParam>(i) + static_cast<TypeParam>(j)) * static_cast<TypeParam>(2)));
        }
    }
}

TYPED_TEST(LazyMatrixTests, ScalarDivisionTest) {
    LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    auto m2 = m / static_cast<TypeParam>(2);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(m2(i, j), (static_cast<TypeParam>(i) + static_cast<TypeParam>(j)) / static_cast<TypeParam>(2)));
        }
    }
}

TYPED_TEST(LazyMatrixTests, MatrixVectorMultiplicationTest) {
    MyBLAS::Vector<TypeParam> v(10, [](size_t i) { return static_cast<TypeParam>(i); });
    MyBLAS::LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        TypeParam expected = 0;
        for (size_t j = 0; j < 10; ++j) {
            expected += (static_cast<TypeParam>(i) + static_cast<TypeParam>(j)) * static_cast<TypeParam>(j);
        }
        EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(result[i], expected));
    }
}

TYPED_TEST(LazyMatrixTests, LazyMatrixLazyVectorMultiplicationTest) {
    MyBLAS::LazyVector<TypeParam> v(10, [](size_t i) { return static_cast<TypeParam>(i); });
    MyBLAS::LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i) + static_cast<TypeParam>(j); });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        TypeParam expected = 0;
        for (size_t j = 0; j < 10; ++j) {
            expected += (static_cast<TypeParam>(i) + static_cast<TypeParam>(j)) * static_cast<TypeParam>(j);
        }
        EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(result[i], expected));
    }
}

TYPED_TEST(LazyMatrixTests, MatrixVectorMultiplicationTest_ScalarMultiplication) {
    MyBLAS::Vector<TypeParam> v(10, 2);
    MyBLAS::LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i + j); });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        TypeParam expected = 0;
        for (size_t j = 0; j < 10; ++j) {
            expected += (static_cast<TypeParam>(i) + static_cast<TypeParam>(j)) * 2;
        }
        EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(result[i], expected));
    }
}

TYPED_TEST(LazyMatrixTests, MatrixVectorMultiplicationTest_ZeroVector) {
    MyBLAS::Vector<TypeParam> v(10, 0.0f);
    MyBLAS::LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return static_cast<TypeParam>(i + j); });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(result[i], 0));
    }
}

TYPED_TEST(LazyMatrixTests, MatrixVectorMultiplicationTest_IdentityMatrix) {
    MyBLAS::Vector<TypeParam> v(10, [](size_t i) { return static_cast<TypeParam>(i); });
    MyBLAS::LazyMatrix<TypeParam> m(10, 10, [](size_t i, size_t j) { return i == j ? 1 : 0; });
    auto result = m * v;
    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_TRUE(LazyMatrixTests<TypeParam>::IsClose(result[i], static_cast<TypeParam>(i)));
    }
}



} //namespace MyBLAS
