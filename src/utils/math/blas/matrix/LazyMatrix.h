/**
* @file LazyMatrix.h
* @brief Header file for the LazyMatrix class.
* @author Arjun Earthperson
* @date 10/13/2023
* @details This file contains the definition of the LazyMatrix class, which is a matrix that lazily computes its values.
 */

#ifndef NE591_008_LAZYMATRIX_H
#define NE591_008_LAZYMATRIX_H

#include <algorithm>
#include <cstddef>
#include <functional>

#include "math/blas/matrix/ContainerExpression.h"
#include "math/blas/matrix/ElementwiseExpression.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/LazyVector.h"
#include "math/blas/vector/MatrixVectorExpression.h"

namespace MyBLAS {

/**
 * @class LazyMatrix
 * @brief A matrix that lazily computes its values.
 *
 * This class represents a matrix whose values are computed on-the-fly each time they're accessed.
 * The computation is defined by a generator function provided at construction time.
 *
 * Note: This implementation does not cache computed values. Even though we're using the lazy evaluation
 * pattern here, we've made the choice to ignore caching of computed values for simplicity and to save memory.
 * This could be less efficient if the computation is expensive and the same values are accessed multiple times.
 * The choice between this approach and caching computed values depends on the specific requirements of your
 * application.
 */
template <typename DataType>
class LazyMatrix {
  public:
    /**
     * @brief Type alias for the generator function.
     *
     * The generator function takes two size_t parameters (representing the row and column indices)
     * and returns a value of the DataType.
     */
    using Generator = std::function<DataType(size_t, size_t)>;

    /**
     * @brief Constructor for the LazyMatrix class.
     *
     * @param rows The number of rows in the matrix.
     * @param cols The number of columns in the matrix.
     * @param generator The generator function used to compute the values of the matrix.
     */
    LazyMatrix(size_t rows, size_t cols, Generator generator)
        : rows_(rows), cols_(cols), generator_(std::move(generator)) {}

    /**
     * @brief Returns the number of rows in the matrix.
     *
     * @return The number of rows in the matrix.
     */
    [[nodiscard]] virtual size_t getRows() const { return rows_; }

    /**
     * @brief Returns the number of columns in the matrix.
     *
     * @return The number of columns in the matrix.
     */
    [[nodiscard]] virtual size_t getCols() const { return cols_; }

    /**
     * @brief Computes and returns the value at the specified row and column.
     *
     * @param row The row index.
     * @param col The column index.
     * @return The computed value at the specified row and column.
     */
    virtual DataType operator()(size_t row, size_t col) const {
        return generator_(row, col);
    }

    /**
     * @brief Default constructor for the LazyMatrix class.
     */
    LazyMatrix() = default;

    /**
     * @brief Destructor for the LazyMatrix class.
     */
    virtual ~LazyMatrix() = default;

    /**
     * @brief Copy constructor for the LazyMatrix class.
     *
     * @param other The LazyMatrix object to be copied.
     */
    LazyMatrix(const LazyMatrix& other) : rows_(other.rows_), cols_(other.cols_), generator_(other.generator_) {}

    /**
     * @brief Copy assignment operator for the LazyMatrix class.
     *
     * @param other The LazyMatrix object to be copied.
     * @return A reference to the current object.
     */
    LazyMatrix& operator=(const LazyMatrix& other) {
        if (this != &other) {
            rows_ = other.rows_;
            cols_ = other.cols_;
            generator_ = other.generator_;
        }
        return *this;
    }

    /**
     * Matrix-Matrix Multiplication
     */
    // TODO::DOCUMENT
    // MyBLAS::Matrix--MyBLAS::LazyMatrix multiplication, return MyBLAS::Matrix
    friend LazyMatrix operator*(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        return ContainerExpression<MyBLAS::Matrix, LazyMatrix, DataType>::multiplyMatrixMatrixType2(a, b);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix multiplication, return MyBLAS::Matrix
    friend LazyMatrix operator*(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        return ContainerExpression<LazyMatrix, MyBLAS::Matrix, DataType>::multiplyMatrixMatrixType1(a, b);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix-MyBLAS::LazyMatrix multiplication, return MyBLAS::LazyMatrix
    friend LazyMatrix operator*(LazyMatrix const& a, LazyMatrix const& b) {
        return ContainerExpression<LazyMatrix, LazyMatrix, DataType>::multiplyMatrixMatrixType2(a, b);
    }

    /**
     * Matrix-Vector Multiplication
     */
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::Vector multiplication, return MyBLAS::Vector
    friend MyBLAS::Vector<DataType> operator*(LazyMatrix const& a, MyBLAS::Vector<DataType> const& b) {
        return MatrixVectorExpression<LazyMatrix, MyBLAS::Vector, DataType>::multiplyMatrixVector(a, b);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix-MyBLAS::LazyVector multiplication, return MyBLAS::LazyVector
    friend LazyVector<DataType> operator*(LazyMatrix const& a, LazyVector<DataType> const& b) {
        return MatrixVectorExpression<LazyMatrix, LazyVector, DataType>::multiplyMatrixVector(a, b);
    }
    // TODO::DOCUMENT
    // MyBLAS::LazyVector--MyBLAS::LazyMatrix multiplication, NOT IMPLEMENTED
    // TODO::DOCUMENT
    // MyBLAS::Vector--MyBLAS::LazyMatrix multiplication, NOT IMPLEMENTED

    /**
     * Matrix-Matrix Elementwise Operations
     */
    /** Define the addition operation. **/
    // TODO::DOCUMENT
    // MyBLAS::Matrix--MyBLAS::LazyMatrix addition, return MyBLAS::Matrix
    friend LazyMatrix operator+(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x + y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix addition, return MyBLAS::Matrix
    friend LazyMatrix operator+(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y + x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix addition, return MyBLAS::LazyMatrix
    friend LazyMatrix operator+(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x + y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    /** Define the subtraction operation. **/
    // TODO::DOCUMENT
    // MyBLAS::Matrix--MyBLAS::LazyMatrix subtraction, return MyBLAS::Matrix
    friend LazyMatrix operator-(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x - y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix subtraction, return MyBLAS::Matrix
    friend LazyMatrix operator-(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y - x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix subtraction, return MyBLAS::LazyMatrix
    friend LazyMatrix operator-(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x - y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    /** Define the equality operation. **/
    // TODO::DOCUMENT
    // MyBLAS::Matrix--MyBLAS::LazyMatrix equality, return MyBLAS::Matrix
    friend LazyMatrix operator==(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x == y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix equality, return MyBLAS::Matrix
    friend LazyMatrix operator==(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y == x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix equality, return MyBLAS::LazyMatrix
    friend LazyMatrix operator==(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x == y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    /** Define the inequality operation. **/
    // TODO::DOCUMENT
    // MyBLAS::Matrix--MyBLAS::LazyMatrix inequality, return MyBLAS::Matrix
    friend LazyMatrix operator!=(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x != y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix inequality, return MyBLAS::Matrix
    friend LazyMatrix operator!=(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y != x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    // TODO::DOCUMENT
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix inequality, return MyBLAS::LazyMatrix
    friend LazyMatrix operator!=(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x != y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * Matrix-Scalar Operations
     */
    // Define the scalar addition operation.
    // TODO::DOCUMENT
    LazyMatrix operator+(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) + scalar; });
    }
    // Define the scalar subtraction operation.
    // TODO::DOCUMENT
    LazyMatrix operator-(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) - scalar; });
    }
    // Define the scalar multiplication operation.
    // TODO::DOCUMENT
    LazyMatrix operator*(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) * scalar; });
    }
    // Define the scalar division operation.
    // TODO::DOCUMENT
    LazyMatrix operator/(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) / scalar; });
    }
    // Define the unary negation operation.
    // TODO::DOCUMENT
    LazyMatrix operator-() const {
        return LazyMatrix(getRows(), getCols(), [this](size_t i, size_t j) { return -(*this)(i, j); });
    }
    // Define the in-place addition operation.
    // TODO::DOCUMENT
    LazyMatrix& operator+=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) + b(i, j); };
        return *this;
    }
    // Define the in-place subtraction operation.
    // TODO::DOCUMENT
    LazyMatrix& operator-=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) - b(i, j); };
        return *this;
    }
    // Define the in-place multiplication operation.
    // TODO::DOCUMENT
    LazyMatrix& operator*=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) * b(i, j); };
        return *this;
    }
    // Define the in-place division operation.
    // TODO::DOCUMENT
    LazyMatrix& operator/=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) / b(i, j); };
        return *this;
    }
    // Define the in-place scalar addition operation.
    // TODO::DOCUMENT
    LazyMatrix& operator+=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) + scalar; };
        return *this;
    }
    // Define the in-place scalar subtraction operation.
    // TODO::DOCUMENT
    LazyMatrix& operator-=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) - scalar; };
        return *this;
    }
    // Define the in-place scalar multiplication operation.
    // TODO::DOCUMENT
    LazyMatrix& operator*=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) * scalar; };
        return *this;
    }
    // Define the in-place scalar division operation.
    // TODO::DOCUMENT
    LazyMatrix& operator/=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) / scalar; };
        return *this;
    }

    // Define the element-wise equality comparison operation.
    // TODO::DOCUMENT
    friend LazyMatrix<bool> equal(LazyMatrix const& a, LazyMatrix const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x == y; };
        ElementwiseExpression<bool, LazyMatrix, LazyMatrix, decltype(binary_op)> expr(a, b, binary_op);
        return LazyMatrix<bool>(a.getRows(), a.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }
    // Define the equality comparison operation.
    // TODO::DOCUMENT
    friend bool allElementsEqual(LazyMatrix const& a, LazyMatrix const& b) {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
            return false;
        }
        auto eq = equal(a, b);
        for (size_t i = 0; i < eq.getRows(); ++i) {
            for (size_t j = 0; j < eq.getCols(); ++j) {
                if (!eq(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    // TODO::DOCUMENT
    class Proxy {
      public:
        // TODO::DOCUMENT
        Proxy(LazyMatrix& matrix, size_t row) : matrix_(matrix), row_(row) {}
        // TODO::DOCUMENT
        DataType operator[](size_t col) const {
            return matrix_(row_, col);
        }

      private:
        // TODO::DOCUMENT
        LazyMatrix& matrix_;
        // TODO::DOCUMENT
        size_t row_;
    };
    // TODO::DOCUMENT
    class ConstProxy {
      public:
        // TODO::DOCUMENT
        ConstProxy(const LazyMatrix& matrix, size_t row) : matrix_(matrix), row_(row) {}
        // TODO::DOCUMENT
        DataType operator[](size_t col) const {
            return matrix_(row_, col);
        }

      private:
        // TODO::DOCUMENT
        const LazyMatrix& matrix_;
        // TODO::DOCUMENT
        size_t row_;
    };
    // TODO::DOCUMENT
    Proxy operator[](size_t row) {
        return Proxy(*this, row);
    }
    // TODO::DOCUMENT
    ConstProxy operator[](size_t row) const {
        return ConstProxy(*this, row);
    }

  private:
    // TODO::DOCUMENT
    size_t rows_;
    // TODO::DOCUMENT
    size_t cols_;
    // TODO::DOCUMENT
    Generator generator_;

  protected:
    // TODO::DOCUMENT
    void setRows(size_t rows) { rows_ = rows; }
    // TODO::DOCUMENT
    void setCols(size_t cols) { cols_ = cols; }
    // TODO::DOCUMENT
    void setGenerator(const Generator &generator) { generator_ = generator; }
};

// vector-matrix multiplication is not implemented for MyBLAS::Vector<DataType>
// TODO::DOCUMENT
template <typename DataType>
MyBLAS::Vector<DataType> operator*(MyBLAS::Vector<DataType> const& a, LazyMatrix<DataType> const& b) = delete;
}

#endif // NE591_008_LAZYMATRIX_H
