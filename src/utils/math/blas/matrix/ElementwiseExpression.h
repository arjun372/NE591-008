/**
* @file ElementwiseExpression.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
 */

#ifndef NE591_008_ELEMENTWISEEXPRESSION_H
#define NE591_008_ELEMENTWISEEXPRESSION_H

#include <cassert>
#include <cstddef>

namespace MyBLAS {
template <typename DataType, typename ContainerType1, typename ContainerType2, typename BinaryOp>
class ElementwiseExpression {
  public:
    ElementwiseExpression(ContainerType1 const& op1, ContainerType2 const& op2, BinaryOp binary_op)
        : op1_(op1), op2_(op2), binary_op_(binary_op) {}

    DataType operator()(size_t i) const { return binary_op_(op1_(i), op2_(i)); }
    DataType operator()(size_t i, size_t j) const { return binary_op_(op1_(i, j), op2_(i, j)); }

    static ContainerType2 vector(const ContainerType1& a, const ContainerType2& b, BinaryOp op) {
        assert(a.size() == b.size());
        ElementwiseExpression<DataType, ContainerType1, ContainerType2, decltype(op)> expr(a, b, op);
        return ContainerType2(a.size(), [expr](size_t i) { return expr(i); });
    }

    static ContainerType2 matrix(const ContainerType1& a, const ContainerType2& b, BinaryOp op) {
        assert(a.getRows() == b.getRows());
        assert(a.getCols() == b.getCols());
        ElementwiseExpression<DataType, ContainerType1, ContainerType2, decltype(op)> expr(a, b, op);
        return ContainerType2(a.getRows(), b.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }

  private:
    ContainerType1 const& op1_;
    ContainerType2 const& op2_;
    BinaryOp binary_op_;
};
}

#endif // NE591_008_ELEMENTWISEEXPRESSION_H
