/**
* @file ContainerExpression.h
* @brief Header file for the ContainerExpression class.
* @author Arjun Earthperson
* @date 10/13/2023
* @details This file contains the definition of the ContainerExpression class, which is used to perform operations on
* two containers of data.
*/

#ifndef NE591_008_CONTAINEREXPRESSION_H
#define NE591_008_CONTAINEREXPRESSION_H

#include <cstddef>

/**
 * @namespace MyBLAS
 * @brief Namespace for the Basic Linear Algebra Subprograms (BLAS) operations.
 */
namespace MyBLAS {

/**
 * @class ContainerExpression
 * @brief A class to perform operations on two containers of data.
 * @tparam ContainerType1 The type of the first container.
 * @tparam ContainerType2 The type of the second container.
 * @tparam T The type of the elements in the containers.
 */
template <template <typename> class ContainerType1, template <typename> class ContainerType2, typename T>
class ContainerExpression {
  public:
    /**
     * @brief Constructor for the ContainerExpression class.
     * @param matrix1 The first container.
     * @param matrix2 The second container.
     */
    ContainerExpression(const ContainerType1<T> &matrix1, const ContainerType2<T> &matrix2)
        : container1_(matrix1), container2_(matrix2) {}

    /**
     * @brief Overloaded function call operator to perform matrix multiplication.
     * @param i The row index.
     * @param k The column index.
     * @return The result of the multiplication.
     */
    T operator()(size_t i, size_t k) const {
        T result = 0;
        for (size_t j = 0; j < container1_.getCols(); ++j) {
            result += container1_(i, j) * container2_(j, k);
        }
        return result;
    }

    /**
     * @brief Overloaded function call operator to perform matrix-vector multiplication.
     * @param i The index.
     * @return The result of the multiplication.
     */
    T operator()(size_t i) const {
        T result = 0;
        for (size_t j = 0; j < container1_.getCols(); ++j) {
            result += container1_(i, j) * container2_[j];
        }
        return result;
    }

    /**
     * @brief Static function to perform matrix multiplication and return a container of type 2.
     * @param a The first matrix.
     * @param b The second matrix.
     * @return The result of the multiplication.
     */
    static ContainerType2<T> multiplyMatrixMatrixType2(const ContainerType1<T> &a, const ContainerType2<T> &b) {
        ContainerExpression expr(a, b);
        return ContainerType2<T>(a.getRows(), b.getCols(), [expr](size_t i, size_t k) { return expr(i, k); });
    }

    /**
     * @brief Static function to perform matrix multiplication and return a container of type 1.
     * @param a The first matrix.
     * @param b The second matrix.
     * @return The result of the multiplication.
     */
    static ContainerType1<T> multiplyMatrixMatrixType1(const ContainerType1<T> &a, const ContainerType2<T> &b) {
        ContainerExpression expr(a, b);
        return ContainerType1<T>(a.getRows(), b.getCols(), [expr](size_t i, size_t k) { return expr(i, k); });
    }

    /**
     * @brief Static function to perform matrix-vector multiplication.
     * @param a The matrix.
     * @param b The vector.
     * @return The result of the multiplication.
     */
    static ContainerType2<T> multiplyMatrixVector(const ContainerType1<T> &a, const ContainerType2<T> &b) {
        ContainerExpression expr(a, b);
        return ContainerType2<T>(a.size(), [expr](size_t i) { return expr(i); });
    }

  private:
    /**
     * @brief The first container.
     */
    const ContainerType1<T> &container1_;
    /**
     * @brief The second container.
     */
    const ContainerType2<T> &container2_;
};
}
#endif // NE591_008_CONTAINEREXPRESSION_H
