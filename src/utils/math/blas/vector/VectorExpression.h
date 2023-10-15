/**
* @file VectorExpression.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
 */

#ifndef NE591_008_VECTOREXPRESSION_H
#define NE591_008_VECTOREXPRESSION_H

#include <cstddef>

namespace MyBLAS {
template <typename DataType, typename Op1, typename Op2, typename BinaryOp>
class VectorExpression {
  public:
    VectorExpression(Op1 const& op1, Op2 const& op2, BinaryOp binary_op)
        : op1_(op1), op2_(op2), binary_op_(binary_op) {}

    DataType operator[](size_t i) const { return binary_op_(op1_[i], op2_[i]); }

    [[nodiscard]] size_t size() const { return op1_.size(); }  // Assume both operands are the same size.

  private:
    Op1 const& op1_;
    Op2 const& op2_;
    BinaryOp binary_op_;
};
}

#endif // NE591_008_VECTOREXPRESSION_H
