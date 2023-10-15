/**
* @file VectorExpressionTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief TODO::DOCUMENT
 */

#include "math/blas/vector/VectorExpression.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

class VectorExpressionTests : public ::testing::Test {
};

TEST_F(VectorExpressionTests, TestVectorExpression) {
    // Define two vectors as operands
    std::vector<int> op1 = {1, 2, 3, 4, 5};
    std::vector<int> op2 = {5, 4, 3, 2, 1};

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a * b; };

    // Create a VectorExpression
    VectorExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator[]
    EXPECT_EQ(expr[1], 8);  // 2 * 4 = 8
    EXPECT_EQ(expr[3], 8);  // 4 * 2 = 8
}

TEST_F(VectorExpressionTests, TestVectorExpressionWithDifferentBinaryOp) {
    // Define two vectors as operands
    std::vector<int> op1 = {1, 2, 3, 4, 5};
    std::vector<int> op2 = {5, 4, 3, 2, 1};

    // Define a different binary operation
    auto binary_op = [](auto a, auto b) { return a + b; };

    // Create a VectorExpression
    VectorExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator[]
    EXPECT_EQ(expr[1], 6);  // 2 + 4 = 6
    EXPECT_EQ(expr[3], 6);  // 4 + 2 = 6
}

TEST_F(VectorExpressionTests, TestVectorExpressionMultiplication) {
    // Define two vectors as operands
    std::vector<int> op1 = {1, 2, 3, 4, 5};
    std::vector<int> op2 = {5, 4, 3, 2, 1};

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a * b; };

    // Create a VectorExpression
    VectorExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator[]
    EXPECT_EQ(expr[0], 5);  // 1 * 5 = 5
    EXPECT_EQ(expr[1], 8);  // 2 * 4 = 8
    EXPECT_EQ(expr[2], 9);  // 3 * 3 = 9
    EXPECT_EQ(expr[3], 8);  // 4 * 2 = 8
    EXPECT_EQ(expr[4], 5);  // 5 * 1 = 5
}

TEST_F(VectorExpressionTests, TestVectorExpressionAddition) {
    // Define two vectors as operands
    std::vector<int> op1 = {1, 2, 3, 4, 5};
    std::vector<int> op2 = {5, 4, 3, 2, 1};

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a + b; };

    // Create a VectorExpression
    VectorExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator[]
    EXPECT_EQ(expr[0], 6);  // 1 + 5 = 6
    EXPECT_EQ(expr[1], 6);  // 2 + 4 = 6
    EXPECT_EQ(expr[2], 6);  // 3 + 3 = 6
    EXPECT_EQ(expr[3], 6);  // 4 + 2 = 6
    EXPECT_EQ(expr[4], 6);  // 5 + 1 = 6
}

TEST_F(VectorExpressionTests, TestVectorExpressionSubtraction) {
    // Define two vectors as operands
    std::vector<int> op1 = {1, 2, 3, 4, 5};
    std::vector<int> op2 = {5, 4, 3, 2, 1};

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a - b; };

    // Create a VectorExpression
    VectorExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator[]
    EXPECT_EQ(expr[0], -4);  // 1 - 5 = -4
    EXPECT_EQ(expr[1], -2);  // 2 - 4 = -2
    EXPECT_EQ(expr[2], 0);   // 3 - 3 = 0
    EXPECT_EQ(expr[3], 2);   // 4 - 2 = 2
    EXPECT_EQ(expr[4], 4);   // 5 - 1 = 4
}

TEST_F(VectorExpressionTests, TestVectorExpressionDivision) {
    // Define two vectors as operands
    std::vector<double> op1 = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> op2 = {5.0, 4.0, 3.0, 2.0, 1.0};

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a / b; };

    // Create a VectorExpression
    VectorExpression<double, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator[]
    EXPECT_NEAR(expr[0], 0.2, TOLERANCE);  // 1.0 / 5.0 = 0.2
    EXPECT_NEAR(expr[1], 0.5, TOLERANCE);  // 2.0 / 4.0 = 0.5
    EXPECT_NEAR(expr[2], 1.0, TOLERANCE);  // 3.0 / 3.0 = 1.0
    EXPECT_NEAR(expr[3], 2.0, TOLERANCE);  // 4.0 / 2.0 = 2.0
    EXPECT_NEAR(expr[4], 5.0, TOLERANCE);  // 5.0 / 1.0 = 5.0
}

TEST_F(VectorExpressionTests, TestVectorExpressionWithDifferentVectorTypes) {
    // Define two vectors as operands
    std::vector<int> op1 = {1, 2, 3, 4, 5};
    std::array<int, 5> op2 = {5, 4, 3, 2, 1};

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a * b; };

    // Create a VectorExpression
    VectorExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator[]
    EXPECT_EQ(expr[0], 5);  // 1 * 5 = 5
    EXPECT_EQ(expr[1], 8);  // 2 * 4 = 8
    EXPECT_EQ(expr[2], 9);  // 3 * 3 = 9
    EXPECT_EQ(expr[3], 8);  // 4 * 2 = 8
    EXPECT_EQ(expr[4], 5);  // 5 * 1 = 5
}

TEST_F(VectorExpressionTests, TestVectorExpressionWithEmptyVectors) {
    // Define two empty vectors as operands
    std::vector<int> op1 = {};
    std::vector<int> op2 = {};

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a * b; };

    // Create a VectorExpression
    VectorExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the size
    EXPECT_EQ(expr.size(), 0);
}

}
