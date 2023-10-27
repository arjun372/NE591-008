/**
* @file MatrixVectorExpression.h
* @brief Header file for the MatrixVectorExpression class.
* @author Arjun Earthperson
* @date 10/13/2023
*
* This file contains the declaration of the MatrixVectorExpression class, which represents an expression
* for the element-wise multiplication of a matrix and a vector.
 */

#ifndef NE591_008_MATRIXVECTOREXPRESSION_H
#define NE591_008_MATRIXVECTOREXPRESSION_H

#include <cstddef>

/**
 * @class MatrixVectorExpression
 * @brief Represents an expression for the element-wise multiplication of a matrix and a vector.
 *
 * This class defines an expression for the element-wise multiplication of a matrix and a vector. It can be used
 * to perform element-wise multiplication without explicitly creating a new matrix.
 *
 * @tparam MatrixType The template class representing the matrix type.
 * @tparam VectorType The template class representing the vector type.
 * @tparam T The data type of the elements.
 */
template <template <typename> class MatrixType, template <typename> class VectorType, typename T>
class MatrixVectorExpression {
  public:
    /**
     * @brief Constructs a MatrixVectorExpression with a matrix and a vector.
     * @param matrix The matrix for the element-wise multiplication.
     * @param vector The vector for the element-wise multiplication.
     */
    MatrixVectorExpression(const MatrixType<T>& matrix, const VectorType<T>& vector)
        : matrix_(matrix), vector_(vector) {}

    /**
     * @brief Computes the element-wise multiplication of the matrix and vector for a specific row.
     * @param i The index of the row to compute the result for.
     * @return The result of element-wise multiplication for the given row.
     */
    T operator()(size_t i) const {
        T result = 0;
        for (size_t j = 0; j < matrix_.getCols(); ++j) {
            result += matrix_(i, j) * vector_[j];
        }
        return result;
    }

    /**
     * @brief Performs element-wise matrix-vector multiplication.
     * @param a The matrix for multiplication.
     * @param b The vector for multiplication.
     * @return A VectorType containing the result of the element-wise multiplication.
     */
    static VectorType<T> multiplyMatrixVector(const MatrixType<T>& a, const VectorType<T>& b) {
        MatrixVectorExpression expr(a, b);
        VectorType result = VectorType<T>(a.getRows(), [expr](size_t i) { return expr(i); });
        return result;
    }

  private:
    const MatrixType<T>& matrix_; ///< The matrix for element-wise multiplication.
    const VectorType<T>& vector_; ///< The vector for element-wise multiplication.
};

#endif // NE591_008_MATRIXVECTOREXPRESSION_H
