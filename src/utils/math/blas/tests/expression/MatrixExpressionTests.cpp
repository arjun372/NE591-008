/**
* @file MatrixExpressionTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief TODO::DOCUMENT
 */

#include "math/blas/matrix/MatrixExpression.h"

#include <cmath>
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

class MatrixExpressionTests : public ::testing::Test {
};

TEST_F(MatrixExpressionTests, TestMatrixExpression) {
    // Define two simple lambda functions as operands
    auto op1 = [](size_t i, size_t j) { return i + j; };
    auto op2 = [](size_t i, size_t j) { return i - j; };

    // Define a binary operation
    auto binary_op = [](auto a, auto b) { return a * b; };

    // Create a MatrixExpression
    MatrixExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator()
    EXPECT_EQ(expr(1, 2), -3);  // (1+2) * (1-2) = -3
    EXPECT_EQ(expr(3, 4), -7);  // (3+4) * (3-4) = -7
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithDifferentBinaryOp) {
    // Define two simple lambda functions as operands
    auto op1 = [](size_t i, size_t j) { return i + j; };
    auto op2 = [](size_t i, size_t j) { return i - j; };

    // Define a different binary operation
    auto binary_op = [](auto a, auto b) { return a + b; };

    // Create a MatrixExpression
    MatrixExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    // Test the operator()
    EXPECT_EQ(expr(1, 2), 2);  // (1+2) + (1-2) = 2
    EXPECT_EQ(expr(3, 4), 6);  // (3+4) + (3-4) = 6
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithMultiplication) {
    auto op1 = [](size_t i, size_t j) { return i * j; };
    auto op2 = [](size_t i, size_t j) { return i + j; };
    auto binary_op = [](auto a, auto b) { return a * b; };

    MatrixExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_EQ(expr(2, 3), 30);  // (2*3) * (2+3) = 30
    EXPECT_EQ(expr(4, 5), 180);  // (4*5) * (4+5) = 180
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithDivision) {
    auto op1 = [](size_t i, size_t j) { return static_cast<double>(i + j); };
    auto op2 = [](size_t i, size_t j) { return static_cast<double>(i * j); };
    auto binary_op = [](auto a, auto b) { return a / b; };

    MatrixExpression<double, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_NEAR(expr(2, 3), 5.0/6.0, TOLERANCE);  // (2+3) / (2*3) = 5/6
    EXPECT_NEAR(expr(4, 5), 9.0/20.0, TOLERANCE);  // (4+5) / (4*5) = 9/20
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithModulo) {
    auto op1 = [](size_t i, size_t j) { return i + j; };
    auto op2 = [](size_t i, size_t j) { return i * j; };
    auto binary_op = [](auto a, auto b) { return a % b; };

    MatrixExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_EQ(expr(2, 3), 5);  // (2+3) % (2*3) = 5
    EXPECT_EQ(expr(4, 5), 9);  // (4+5) % (4*5) = 9
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithPower) {
    auto op1 = [](size_t i, size_t j) { return i + j; };
    auto op2 = [](size_t i, size_t j) { return i * j; };
    auto binary_op = [](auto a, auto b) { return std::pow(a, b); };

    MatrixExpression<double, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_DOUBLE_EQ(expr(2, 3), 15625);  // (2+3) ^ (2*3) = 15625
    EXPECT_DOUBLE_EQ(expr(4, 5), 1.2157665459056929e+19);  // (4+5) ^ (4*5) = 1.2157665459056929e+19
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithMax) {
    auto op1 = [](size_t i, size_t j) { return i + j; };
    auto op2 = [](size_t i, size_t j) { return i * j; };
    auto binary_op = [](auto a, auto b) { return std::max(a, b); };

    MatrixExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_EQ(expr(2, 3), 6);  // max(2+3, 2*3) = 6
    EXPECT_EQ(expr(4, 5), 20);  // max(4+5, 4*5) = 20
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithMin) {
    auto op1 = [](size_t i, size_t j) { return i + j; };
    auto op2 = [](size_t i, size_t j) { return i * j; };
    auto binary_op = [](auto a, auto b) { return std::min(a, b); };

    MatrixExpression<int, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_EQ(expr(2, 3), 5);  // min(2+3, 2*3) = 5
    EXPECT_EQ(expr(4, 5), 9);  // min(4+5, 4*5) = 9
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithSqrt) {
    auto op1 = [](size_t i, size_t j) { return i * i + j * j; };
    auto op2 = [](size_t i, size_t j) { return i * j; };
    auto binary_op = [](auto a, auto b) { return std::sqrt(a + b); };

    MatrixExpression<double, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_DOUBLE_EQ(expr(2, 3), 4.358898943540674);  // sqrt((2*2 + 3*3) + (2*3)) = 4.358898943540674
    EXPECT_DOUBLE_EQ(expr(4, 5), 7.810249675906654);  // sqrt((4*4 + 5*5) + (4*5)) = 7.810249675906654
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithLog) {
    auto op1 = [](size_t i, size_t j) { return i * i + j * j; };
    auto op2 = [](size_t i, size_t j) { return i * j + 1; };
    auto binary_op = [](auto a, auto b) { return std::log(a + b); };

    MatrixExpression<double, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_DOUBLE_EQ(expr(2, 3), 2.9957322735539909);  // log((2*2 + 3*3) + (2*3 + 1)) = 2.9957322735539909
    EXPECT_DOUBLE_EQ(expr(4, 5), 4.1271343850450917);  // log((4*4 + 5*5) + (4*5 + 1)) = 4.1271343850450917
}

TEST_F(MatrixExpressionTests, TestMatrixExpressionWithExp) {
    auto op1 = [](size_t i, size_t j) { return static_cast<double>(i + j) / 10.0; };
    auto op2 = [](size_t i, size_t j) { return static_cast<double>(i * j) / 10.0; };
    auto binary_op = [](auto a, auto b) { return std::exp(a - b); };

    MatrixExpression<double, decltype(op1), decltype(op2), decltype(binary_op)> expr(op1, op2, binary_op);

    EXPECT_DOUBLE_EQ(expr(2, 3), std::exp((2.0 + 3.0) / 10.0 - (2.0 * 3.0) / 10.0));  // exp((2+3)/10 - (2*3)/10)
    EXPECT_DOUBLE_EQ(expr(4, 5), std::exp((4.0 + 5.0) / 10.0 - (4.0 * 5.0) / 10.0));  // exp((4+5)/10 - (4*5)/10)
}

}
