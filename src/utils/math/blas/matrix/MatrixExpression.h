/**
* @file MatrixExpression.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
 */

#ifndef NE591_008_MATRIXEXPRESSION_H
#define NE591_008_MATRIXEXPRESSION_H

#include <cstddef>

namespace MyBLAS {
template <typename DataType, typename Op1, typename Op2, typename BinaryOp>
class MatrixExpression {
  public:
    MatrixExpression(Op1 const& op1, Op2 const& op2, BinaryOp binary_op)
        : op1_(op1), op2_(op2), binary_op_(binary_op) {}

    DataType operator()(size_t i, size_t j) const { return binary_op_(op1_(i, j), op2_(i, j)); }

    [[nodiscard]] size_t rows() const { return op1_.getRows(); }  // Assume both operands have the same number of rows.
    [[nodiscard]] size_t getCols() const { return op1_.getCols(); }  // Assume both operands have the same number of columns.

  private:
    Op1 const& op1_;
    Op2 const& op2_;
    BinaryOp binary_op_;
};
}

#endif // NE591_008_MATRIXEXPRESSION_H
