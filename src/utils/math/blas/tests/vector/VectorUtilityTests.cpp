/**
* @file VectorUtilityTests.cpp
* @author Arjun Earthperson
* @date 10/27/2023
*
* @brief This file contains unit tests for the utility methods associated with the MyBLAS::Vector class.
 */

#include "math/blas/vector/Vector.h"
#include "math/blas/BLAS.h"
#include "math/blas/Ops.h"
#include <gtest/gtest.h>

#define TOLERANCE 1.0e-6

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<float, double, long double> NumericTypes;
TYPED_TEST_SUITE(VectorUtilityTests, NumericTypes);

template <typename T> class VectorUtilityTests : public ::testing::Test {};

// Test for isZeroVector
TYPED_TEST(VectorUtilityTests, IsZeroVectorTest) {
    Vector<TypeParam> zeroVector(3, 0);
    Vector<TypeParam> nonZeroVector({1, 2, 3});
    EXPECT_TRUE(isZeroVector(zeroVector));
    EXPECT_FALSE(isZeroVector(nonZeroVector));
}

// Test for isUnitVector
TYPED_TEST(VectorUtilityTests, IsUnitVectorTest) {
    Vector<TypeParam> unitVector({0, 1, 0});
    Vector<TypeParam> nonUnitVector({1, 2, 3});
    EXPECT_TRUE(isUnitVector(unitVector));
    EXPECT_FALSE(isUnitVector(nonUnitVector));
}

// Test for vectorNorm
TYPED_TEST(VectorUtilityTests, VectorNormTest) {
    Vector<TypeParam> vector({1, 2, 3});
    auto expectedNorm = static_cast<double>(std::sqrt(14));
    EXPECT_NEAR(static_cast<double>(norm(vector)), expectedNorm, TOLERANCE);
}

// Test for dotProduct
TYPED_TEST(VectorUtilityTests, DotProductTest) {
    Vector<TypeParam> vectorA({1, 2, 3});
    Vector<TypeParam> vectorB({4, 5, 6});
    auto expectedDotProduct = static_cast<double>(32);
    EXPECT_EQ(vectorA * vectorB, expectedDotProduct);
}

}
