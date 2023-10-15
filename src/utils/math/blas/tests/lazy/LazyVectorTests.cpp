/**
* @file LazyVectorTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief TODO::DOCUMENT
*/

#include "math/blas/vector/LazyVector.h"

#include <cmath>
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

class LazyVectorTests : public ::testing::Test {
  protected:
    // Helper function to compare two floating point numbers with a tolerance.
    static bool IsClose(FLOAT_TYPE a, FLOAT_TYPE b, FLOAT_TYPE tolerance = TOLERANCE) {
        return std::abs(a - b) < tolerance;
    }
};

TEST_F(LazyVectorTests, SizeTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    EXPECT_EQ(v.size(), 10);
}

TEST_F(LazyVectorTests, ElementAccessTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i * i); });
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_TRUE(IsClose(v[i], static_cast<FLOAT_TYPE>(i * i)));
    }
}

TEST_F(LazyVectorTests, AdditionTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i * i); });
    auto v3 = v1 + v2;
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_TRUE(IsClose(v3[i], static_cast<FLOAT_TYPE>(i + i * i)));
    }
}

// Test for subtraction operation
TEST_F(LazyVectorTests, SubtractionTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i * i); });
    auto v3 = v1 - v2;
    for (size_t i = 0; i < v3.size(); ++i) {
        const auto fi = static_cast<FLOAT_TYPE>(i);
        const auto value = fi - fi * fi;
        SCOPED_TRACE("i = " + std::to_string(i) + ",v3[i] = "+std::to_string(v3[i])+", value = "+std::to_string(value));
        EXPECT_TRUE(IsClose(v3[i], value));
    }
}

// Test for multiplication operation
TEST_F(LazyVectorTests, MultiplicationTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i * i); });
    auto v3 = v1 * v2;
    for (size_t i = 0; i < v3.size(); ++i) {
        const auto fi = static_cast<FLOAT_TYPE>(i);
        EXPECT_TRUE(IsClose(v3[i], static_cast<FLOAT_TYPE>(fi * fi * fi)));
    }
}

// Test for division operation
TEST_F(LazyVectorTests, DivisionTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return (static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)); });
    auto v3 = v1 / v2;
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_TRUE(IsClose(v3[i], static_cast<FLOAT_TYPE>((static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)) / (static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)))));
    }
}

// Test for scalar addition operation
TEST_F(LazyVectorTests, ScalarAdditionTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    auto v2 = v + 5.0;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_TRUE(IsClose(v2[i], static_cast<FLOAT_TYPE>(i + 5.0)));
    }
}

// Test for scalar subtraction operation
TEST_F(LazyVectorTests, ScalarSubtractionTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    auto v2 = v - 5.0;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_TRUE(IsClose(v2[i], static_cast<FLOAT_TYPE>(i - 5.0)));
    }
}

// Test for scalar multiplication operation
TEST_F(LazyVectorTests, ScalarMultiplicationTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    auto v2 = v * 5.0;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_TRUE(IsClose(v2[i], static_cast<FLOAT_TYPE>(i * 5.0)));
    }
}

// Test for scalar division operation
TEST_F(LazyVectorTests, ScalarDivisionTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    auto v2 = v / 5.0;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_TRUE(IsClose(v2[i], static_cast<FLOAT_TYPE>((static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)) / 5.0)));
    }
}

// Test for division by zero
TEST_F(LazyVectorTests, DivisionByZeroTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(0); });
    auto v3 = v1 / v2;
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_TRUE(std::isinf(v3[i]));
    }
}

// Test for scalar division by zero
TEST_F(LazyVectorTests, ScalarDivisionByZeroTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) {
        return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1);
    });
    auto v2 = v / 0.0;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_TRUE(std::isinf(v2[i]));
    }
}

// Test for checking if subtraction returns the same value as addition with the negation of the value
TEST_F(LazyVectorTests, SubtractionAdditionNegationTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    auto v3 = v1 - v2;
    auto v4 = v1 + (-v2);
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_TRUE(IsClose(v3[i], v4[i]));
    }
}

// Test for unary negation operation
TEST_F(LazyVectorTests, UnaryNegationTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    auto v2 = -v;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_TRUE(IsClose(v2[i], -v[i]));
    }
}

// Test for in-place addition
TEST_F(LazyVectorTests, InPlaceAdditionTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i * i); });
    v1 += v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_TRUE(IsClose(v1[i], static_cast<FLOAT_TYPE>(i + i * i)));
    }
}

// Test for in-place subtraction
TEST_F(LazyVectorTests, InPlaceSubtractionTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i * i); });
    v1 -= v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        const auto fi = static_cast<FLOAT_TYPE>(i);
        const auto value = fi - fi * fi;
        EXPECT_TRUE(IsClose(v1[i], value));
    }
}

// Test for in-place multiplication
TEST_F(LazyVectorTests, InPlaceMultiplicationTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i * i); });
    v1 *= v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        const auto fi = static_cast<FLOAT_TYPE>(i);
        EXPECT_TRUE(IsClose(v1[i], static_cast<FLOAT_TYPE>(fi * fi * fi)));
    }
}

// Test for in-place division
TEST_F(LazyVectorTests, InPlaceDivisionTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return (static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)); });
    v1 /= v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        const auto fi = static_cast<FLOAT_TYPE>(i);
        const auto f1 = static_cast<FLOAT_TYPE>(1);
        EXPECT_TRUE(IsClose(v1[i], static_cast<FLOAT_TYPE>((fi + f1) / (fi * fi + f1))));
    }
}

// Test for element-wise addition assignment (+=)
TEST_F(LazyVectorTests, ElementWiseAdditionAssignmentTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i){ return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i); });
    v1 += v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        const auto fi = static_cast<FLOAT_TYPE>(i);
        EXPECT_TRUE(IsClose(v1[i], static_cast<FLOAT_TYPE>(fi + fi * fi)));
    }
}

// Test for element-wise subtraction assignment (-=)
TEST_F(LazyVectorTests, ElementWiseSubtractionAssignmentTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i); });
    v1 -= v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        const auto fi = static_cast<FLOAT_TYPE>(i);
        EXPECT_TRUE(IsClose(v1[i], static_cast<FLOAT_TYPE>(fi - fi * fi)));
    }
}

// Test for element-wise multiplication assignment (*=)
TEST_F(LazyVectorTests, ElementWiseMultiplicationAssignmentTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i); });
    v1 *= v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_TRUE(IsClose(v1[i], static_cast<FLOAT_TYPE>(i * i * i)));
    }
}

// Test for element-wise division assignment (/=)
TEST_F(LazyVectorTests, ElementWiseDivisionAssignmentTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return (static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)); });
    v1 /= v2;
    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_TRUE(IsClose(v1[i], static_cast<FLOAT_TYPE>((static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)) / (static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)))));
    }
}

// Test for equality and inequality operators
TEST_F(LazyVectorTests, EqualityInequalityTest) {
    LazyVector<FLOAT_TYPE> v1(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v2(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    LazyVector<FLOAT_TYPE> v3(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(i); });

    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
}

// Test for assignment operator
TEST_F(LazyVectorTests, AssignmentTest) {
    MyBLAS::LazyVector<float> v1(10, [](size_t i) { return static_cast<float>(i); });
    MyBLAS::LazyVector<float> v2(10, [](size_t i) { return static_cast<float>(i) * static_cast<float>(i); });

    // Check that v1 and v2 are different
    EXPECT_NE(v1, v2);

    // Assign v2 to v1
    v1 = v2;

    // Now v1 and v2 should be equal
    EXPECT_EQ(v1, v2);
}

// Test for element-wise scalar addition assignment (+=)
TEST_F(LazyVectorTests, ElementWiseScalarAdditionAssignmentTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    v += static_cast<FLOAT_TYPE>(5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_TRUE(IsClose(v[i], static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(5)));
    }
}

// Test for element-wise scalar subtraction assignment (-=)
TEST_F(LazyVectorTests, ElementWiseScalarSubtractionAssignmentTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    v -= static_cast<FLOAT_TYPE>(5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_TRUE(IsClose(v[i], static_cast<FLOAT_TYPE>(i) - static_cast<FLOAT_TYPE>(5)));
    }
}

// Test for element-wise scalar multiplication assignment (*=)
TEST_F(LazyVectorTests, ElementWiseScalarMultiplicationAssignmentTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i); });
    v *= static_cast<FLOAT_TYPE>(5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_TRUE(IsClose(v[i], static_cast<FLOAT_TYPE>(i) * static_cast<FLOAT_TYPE>(5)));
    }
}

// Test for element-wise scalar division assignment (/=)
TEST_F(LazyVectorTests, ElementWiseScalarDivisionAssignmentTest) {
    LazyVector<FLOAT_TYPE> v(10, [](size_t i) { return static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1); });
    v /= static_cast<FLOAT_TYPE>(5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_TRUE(IsClose(v[i], static_cast<FLOAT_TYPE>((static_cast<FLOAT_TYPE>(i) + static_cast<FLOAT_TYPE>(1)) / static_cast<FLOAT_TYPE>(5))));
    }
}

}  // namespace MyBLAS
