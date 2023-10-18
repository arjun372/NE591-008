/**
* @file MatrixExpression.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
 */

#ifndef NE591_008_MATRIXVECTOREXPRESSION_H
#define NE591_008_MATRIXVECTOREXPRESSION_H

#include <cstddef>

template <typename DataType, typename MatrixType, typename VectorType>
class MatrixVectorExpression {
  public:
    MatrixVectorExpression(const MatrixType& matrix, const VectorType& vector)
        : matrix_(matrix), vector_(vector) {}

    DataType operator()(size_t i) const {
        DataType result = 0;
        for (size_t j = 0; j < matrix_.cols(); ++j) {
            result += matrix_(i, j) * vector_[j];
        }
        return result;
    }

    static VectorType multiplyMatrixVector(const MatrixType& a, const VectorType& b) {
        MatrixVectorExpression<DataType, MatrixType, VectorType> expr(a, b);
        VectorType result = VectorType(a.rows());
        for (size_t i = 0; i < a.rows(); ++i) {
            result[i] = expr(i);
        }
        return result;
    }

  private:
    const MatrixType& matrix_;
    const VectorType& vector_;
};



#endif // NE591_008_MATRIXVECTOREXPRESSION_H
