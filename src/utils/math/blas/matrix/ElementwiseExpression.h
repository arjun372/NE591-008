/**
* @file ElementwiseExpression.h
* @brief This file defines a class for performing element-wise operations on two containers.
* @author Arjun Earthperson
* @date 10/13/2023
*
* @details The ElementwiseExpression class is a template class that allows for element-wise operations
* between two containers of the same size. The operation to be performed is passed as a binary
* function object. The class supports both vector and matrix containers.
 */

#ifndef NE591_008_ELEMENTWISEEXPRESSION_H
#define NE591_008_ELEMENTWISEEXPRESSION_H

#include <cassert>
#include <cstddef>

/**
 * @namespace MyBLAS
 * @brief This namespace contains classes and functions for basic linear algebra operations.
 */
namespace MyBLAS {

/**
 * @class ElementwiseExpression
 * @brief A class for performing element-wise operations on two containers.
 *
 * @tparam DataType The type of data stored in the containers.
 * @tparam ContainerType1 The type of the first container.
 * @tparam ContainerType2 The type of the second container.
 * @tparam BinaryOp The type of the binary operation to be performed.
 */
template <typename DataType, typename ContainerType1, typename ContainerType2, typename BinaryOp>
class ElementwiseExpression {
  public:
    /**
     * @brief Constructor for the ElementwiseExpression class.
     *
     * @param op1 The first operand (container).
     * @param op2 The second operand (container).
     * @param binary_op The binary operation to be performed.
     */
    ElementwiseExpression(ContainerType1 const& op1, ContainerType2 const& op2, BinaryOp binary_op)
        : op1_(op1), op2_(op2), binary_op_(binary_op) {}

    /**
     * @brief Function call operator for performing the operation on a specific element of the containers.
     *
     * @param i The index of the element.
     * @return The result of the operation.
     */
    DataType operator()(size_t i) const { return binary_op_(op1_(i), op2_(i)); }

    /**
     * @brief Function call operator for performing the operation on a specific element of the containers (for matrices).
     *
     * @param i The row index of the element.
     * @param j The column index of the element.
     * @return The result of the operation.
     */
    DataType operator()(size_t i, size_t j) const { return binary_op_(op1_(i, j), op2_(i, j)); }

    /**
     * @brief Static function for performing the operation on two vectors.
     *
     * @param a The first vector.
     * @param b The second vector.
     * @param op The operation to be performed.
     * @return A new vector that is the result of the operation.
     */
    static ContainerType2 vector(const ContainerType1& a, const ContainerType2& b, BinaryOp op) {
        assert(a.size() == b.size());
        ElementwiseExpression<DataType, ContainerType1, ContainerType2, decltype(op)> expr(a, b, op);
        return ContainerType2(a.size(), [expr](size_t i) { return expr(i); });
    }

    /**
     * @brief Static function for performing the operation on two matrices.
     *
     * @param a The first matrix.
     * @param b The second matrix.
     * @param op The operation to be performed.
     * @return A new matrix that is the result of the operation.
     */
    static ContainerType2 matrix(const ContainerType1& a, const ContainerType2& b, BinaryOp op) {
        assert(a.getRows() == b.getRows());
        assert(a.getCols() == b.getCols());
        ElementwiseExpression<DataType, ContainerType1, ContainerType2, decltype(op)> expr(a, b, op);
        return ContainerType2(a.getRows(), b.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }

  private:
    /**
     * @brief The first operand (container).
     */
    ContainerType1 const& op1_;

    /**
     * @brief The second operand (container).
     */
    ContainerType2 const& op2_;

    /**
     * @brief The binary operation to be performed.
     */
    BinaryOp binary_op_;
};
}

#endif // NE591_008_ELEMENTWISEEXPRESSION_H
