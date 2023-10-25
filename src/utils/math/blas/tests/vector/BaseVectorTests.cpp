/**
* @file BaseVectorTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief This file contains unit tests for the MyBLAS::Vector class.
 */

#include "math/blas/vector/Vector.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<int, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(VectorTests, NumericTypes);

template <typename T>
class VectorTests : public ::testing::Test {};

TYPED_TEST(VectorTests, ConstructorTest) {
    Vector<TypeParam> v1(3, 1);
    EXPECT_EQ(v1.size(), 3);
    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v1[i], 1);
    }
}

TYPED_TEST(VectorTests, EqualityTest) {
    Vector<TypeParam> v1(3, 1);
    Vector<TypeParam> v2(3, 1);
    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(VectorTests, AdditionTest) {
    Vector<TypeParam> v1(3, 1);
    Vector<TypeParam> v2(3, 2);
    Vector<TypeParam> v3 = v1 + v2;
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_EQ(v3[i], 3);
    }
}

TYPED_TEST(VectorTests, SubtractionTest) {
    Vector<TypeParam> v1(3, 2);
    Vector<TypeParam> v2(3, 1);
    Vector<TypeParam> v3 = v1 - v2;
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_EQ(v3[i], 1);
    }
}

TYPED_TEST(VectorTests, DotProductTest) {
    Vector<TypeParam> v1(3, 2);
    Vector<TypeParam> v2(3, 3);
    TypeParam result = v1 * v2;
    EXPECT_EQ(result, 18);
}

TYPED_TEST(VectorTests, ScalarMultiplicationTest) {
    Vector<TypeParam> v1(3, 2);
    Vector<TypeParam> v2 = v1 * 2;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 4);
    }
}

TYPED_TEST(VectorTests, ScalarDivisionTest) {
    Vector<TypeParam> v1(3, 2);
    Vector<TypeParam> v2 = v1 / 2;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 1);
    }
}

TYPED_TEST(VectorTests, ScalarAdditionTest) {
    Vector<TypeParam> v1(3, 2);
    Vector<TypeParam> v2 = v1 + 2;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 4);
    }
}

TYPED_TEST(VectorTests, ScalarSubtractionTest) {
    Vector<TypeParam> v1(3, 2);
    Vector<TypeParam> v2 = v1 - 1;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 1);
    }
}

TYPED_TEST(VectorTests, EmptyVectorTest) {
    Vector<TypeParam> v1;
    EXPECT_EQ(v1.size(), 0);
}

TYPED_TEST(VectorTests, LambdaConstructorTest) {
    Vector<TypeParam> v1(5, [](size_t i) { return i * 2; });
    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v1[i], i * 2);
    }
}

TYPED_TEST(VectorTests, InequalityTest) {
    Vector<TypeParam> v1(3, 1);
    Vector<TypeParam> v2(3, 2);
    EXPECT_NE(v1, v2);
}

TYPED_TEST(VectorTests, VectorAdditionSizeMismatchTest) {
    Vector<TypeParam> v1(3, 1);
    Vector<TypeParam> v2(4, 2);
   // EXPECT_THROW(v1 + v2, std::exception);
}

TYPED_TEST(VectorTests, VectorSubtractionSizeMismatchTest) {
    Vector<TypeParam> v1(3, 1);
    Vector<TypeParam> v2(4, 2);
    //EXPECT_DEATH(v1 - v2, ".*");
}

TYPED_TEST(VectorTests, VectorDotProductSizeMismatchTest) {
    Vector<TypeParam> v1(3, 1);
    Vector<TypeParam> v2(4, 2);
    Vector<TypeParam> v3(6, 2);
    //EXPECT_THROW(v1 * v2, std::exception);
}

TYPED_TEST(VectorTests, VectorScalarDivisionByZeroTest) {
    Vector<TypeParam> v1(3, 1);
   // EXPECT_THROW(v1 / 0, std::exception);
}

} // namespace MyBLAS
