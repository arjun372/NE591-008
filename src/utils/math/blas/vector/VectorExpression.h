/**
* @file VectorExpression.h
* @brief Header file for the MyBLAS::VectorExpression class.
* @author Arjun Earthperson
* @date 10/13/2023
* @details This file contains the declaration of the MyBLAS::VectorExpression class, which represents an expression
* for element-wise vector operations.
 */

#ifndef NE591_008_VECTOREXPRESSION_H
#define NE591_008_VECTOREXPRESSION_H

#include <cstddef>

namespace MyBLAS {

/**
 * @class VectorExpression
 * @brief Represents an expression for element-wise vector operations.
 *
 * This class defines an expression for performing element-wise binary operations on two vectors.
 *
 * @tparam DataType The data type of the vector elements.
 * @tparam Op1 The first vector operand.
 * @tparam Op2 The second vector operand.
 * @tparam BinaryOp The binary operation to be performed on the vectors.
 */
template <typename DataType, typename Op1, typename Op2, typename BinaryOp>
class VectorExpression {
  public:
    /**
     * @brief Constructor for the VectorExpression.
     * @param op1 The first vector operand.
     * @param op2 The second vector operand.
     * @param binary_op The binary operation to be performed on the vectors.
     */
    VectorExpression(Op1 const& op1, Op2 const& op2, BinaryOp binary_op)
        : op1_(op1), op2_(op2), binary_op_(binary_op) {}

    /**
     * @brief Operator[] to access the result of the element-wise operation at a specific index.
     * @param i The index of the element to access.
     * @return The result of the element-wise operation at the given index.
     */
    DataType operator[](size_t i) const { return binary_op_(op1_[i], op2_[i]); }

    /**
     * @brief Getter for the size of the resulting vector expression.
     * @return The size of the resulting vector expression. Assumes both operands have the same size.
     */
    [[nodiscard]] size_t size() const { return op1_.size(); }  // Assume both operands are the same size.

  private:
    Op1 const& op1_;    ///< The first vector operand.
    Op2 const& op2_;    ///< The second vector operand.
    BinaryOp binary_op_; ///< The binary operation to be performed.
};
}

#endif // NE591_008_VECTOREXPRESSION_H
