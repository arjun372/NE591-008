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

template <template <typename> class MatrixType, template <typename> class VectorType, typename T>
//template <typename DataType, typename MatrixType, typename VectorType>
class MatrixVectorExpression {
  public:
    MatrixVectorExpression(const MatrixType<T>& matrix, const VectorType<T>& vector)
        : matrix_(matrix), vector_(vector) {}

    T operator()(size_t i) const {
        T result = 0;
        for (size_t j = 0; j < matrix_.getCols(); ++j) {
            result += matrix_(i, j) * vector_[j];
        }
        return result;
    }

    static VectorType<T> multiplyMatrixVector(const MatrixType<T>& a, const VectorType<T>& b) {
        MatrixVectorExpression expr(a, b);
        VectorType result = VectorType<T>(a.getRows());
        for (size_t i = 0; i < a.getRows(); ++i) {
            result[i] = expr(i);
        }
        return result;
    }

  private:
    const MatrixType<T>& matrix_;
    const VectorType<T>& vector_;
};



#endif // NE591_008_MATRIXVECTOREXPRESSION_H
