/**
* @file LazyMatrix.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
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
    using Generator = std::function<DataType(size_t, size_t)>;

    LazyMatrix(size_t rows, size_t cols, Generator generator)
        : rows_(rows), cols_(cols), generator_(std::move(generator)) {}

    [[nodiscard]] virtual size_t getRows() const { return rows_; }
    [[nodiscard]] virtual size_t getCols() const { return cols_; }

    virtual DataType operator()(size_t row, size_t col) const {
        return generator_(row, col);
    }

    LazyMatrix() = default;

    virtual ~LazyMatrix() = default;

    // copy constructor
    LazyMatrix(const LazyMatrix& other) : rows_(other.rows_), cols_(other.cols_), generator_(other.generator_) {}

    // Assignment operator
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
    // MyBLAS::Matrix--MyBLAS::LazyMatrix multiplication, return MyBLAS::Matrix
    friend LazyMatrix operator*(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        return ContainerExpression<MyBLAS::Matrix, LazyMatrix, DataType>::multiplyMatrixMatrixType2(a, b);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix multiplication, return MyBLAS::Matrix
    friend LazyMatrix operator*(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        return ContainerExpression<LazyMatrix, MyBLAS::Matrix, DataType>::multiplyMatrixMatrixType1(a, b);
    }
    //  MyBLAS::LazyMatrix-MyBLAS::LazyMatrix multiplication, return MyBLAS::LazyMatrix
    friend LazyMatrix operator*(LazyMatrix const& a, LazyMatrix const& b) {
        return ContainerExpression<LazyMatrix, LazyMatrix, DataType>::multiplyMatrixMatrixType2(a, b);
    }

    /**
     * Matrix-Vector Multiplication
     */
    //  MyBLAS::LazyMatrix--MyBLAS::Vector multiplication, return MyBLAS::Vector
    friend MyBLAS::Vector<DataType> operator*(LazyMatrix const& a, MyBLAS::Vector<DataType> const& b) {
        return MatrixVectorExpression<LazyMatrix, MyBLAS::Vector, DataType>::multiplyMatrixVector(a, b);
    }
    //  MyBLAS::LazyMatrix-MyBLAS::LazyVector multiplication, return MyBLAS::LazyVector
    friend LazyVector<DataType> operator*(LazyMatrix const& a, LazyVector<DataType> const& b) {
        return MatrixVectorExpression<LazyMatrix, LazyVector, DataType>::multiplyMatrixVector(a, b);
    }
    // MyBLAS::LazyVector--MyBLAS::LazyMatrix multiplication, NOT IMPLEMENTED
    // MyBLAS::Vector--MyBLAS::LazyMatrix multiplication, NOT IMPLEMENTED

    /**
     * Matrix-Matrix Elementwise Operations
     */
    /** Define the addition operation. **/
    // MyBLAS::Matrix--MyBLAS::LazyMatrix addition, return MyBLAS::Matrix
    friend LazyMatrix operator+(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x + y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix addition, return MyBLAS::Matrix
    friend LazyMatrix operator+(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y + x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix addition, return MyBLAS::LazyMatrix
    friend LazyMatrix operator+(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x + y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    /** Define the subtraction operation. **/
    // MyBLAS::Matrix--MyBLAS::LazyMatrix subtraction, return MyBLAS::Matrix
    friend LazyMatrix operator-(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x - y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix subtraction, return MyBLAS::Matrix
    friend LazyMatrix operator-(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y - x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix subtraction, return MyBLAS::LazyMatrix
    friend LazyMatrix operator-(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x - y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    /** Define the equality operation. **/
    // MyBLAS::Matrix--MyBLAS::LazyMatrix equality, return MyBLAS::Matrix
    friend LazyMatrix operator==(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x == y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix equality, return MyBLAS::Matrix
    friend LazyMatrix operator==(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y == x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix equality, return MyBLAS::LazyMatrix
    friend LazyMatrix operator==(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x == y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    /** Define the inequality operation. **/
    // MyBLAS::Matrix--MyBLAS::LazyMatrix inequality, return MyBLAS::Matrix
    friend LazyMatrix operator!=(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x != y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::Matrix inequality, return MyBLAS::Matrix
    friend LazyMatrix operator!=(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y != x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }
    //  MyBLAS::LazyMatrix--MyBLAS::LazyMatrix inequality, return MyBLAS::LazyMatrix
    friend LazyMatrix operator!=(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x != y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * Matrix-Scalar Operations
     */
    // Define the scalar addition operation.
    LazyMatrix operator+(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) + scalar; });
    }
    // Define the scalar subtraction operation.
    LazyMatrix operator-(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) - scalar; });
    }
    // Define the scalar multiplication operation.
    LazyMatrix operator*(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) * scalar; });
    }
    // Define the scalar division operation.
    LazyMatrix operator/(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) / scalar; });
    }
    // Define the unary negation operation.
    LazyMatrix operator-() const {
        return LazyMatrix(getRows(), getCols(), [this](size_t i, size_t j) { return -(*this)(i, j); });
    }
    // Define the in-place addition operation.
    LazyMatrix& operator+=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) + b(i, j); };
        return *this;
    }
    // Define the in-place subtraction operation.
    LazyMatrix& operator-=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) - b(i, j); };
        return *this;
    }
    // Define the in-place multiplication operation.
    LazyMatrix& operator*=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) * b(i, j); };
        return *this;
    }
    // Define the in-place division operation.
    LazyMatrix& operator/=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) / b(i, j); };
        return *this;
    }
    // Define the in-place scalar addition operation.
    LazyMatrix& operator+=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) + scalar; };
        return *this;
    }
    // Define the in-place scalar subtraction operation.
    LazyMatrix& operator-=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) - scalar; };
        return *this;
    }
    // Define the in-place scalar multiplication operation.
    LazyMatrix& operator*=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) * scalar; };
        return *this;
    }
    // Define the in-place scalar division operation.
    LazyMatrix& operator/=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) / scalar; };
        return *this;
    }

    // Define the element-wise equality comparison operation.
    friend LazyMatrix<bool> equal(LazyMatrix const& a, LazyMatrix const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x == y; };
        ElementwiseExpression<bool, LazyMatrix, LazyMatrix, decltype(binary_op)> expr(a, b, binary_op);
        return LazyMatrix<bool>(a.getRows(), a.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }
    // Define the equality comparison operation.
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

    class Proxy {
      public:
        Proxy(LazyMatrix& matrix, size_t row) : matrix_(matrix), row_(row) {}

        DataType operator[](size_t col) const {
            return matrix_(row_, col);
        }

      private:
        LazyMatrix& matrix_;
        size_t row_;
    };

    class ConstProxy {
      public:
        ConstProxy(const LazyMatrix& matrix, size_t row) : matrix_(matrix), row_(row) {}

        DataType operator[](size_t col) const {
            return matrix_(row_, col);
        }

      private:
        const LazyMatrix& matrix_;
        size_t row_;
    };

    Proxy operator[](size_t row) {
        return Proxy(*this, row);
    }

    ConstProxy operator[](size_t row) const {
        return ConstProxy(*this, row);
    }

  private:
    size_t rows_;
    size_t cols_;
    Generator generator_;

  protected:
    void setRows(size_t rows) { rows_ = rows; }
    void setCols(size_t cols) { cols_ = cols; }
    void setGenerator(const Generator &generator) { generator_ = generator; }
};

// vector-matrix multiplication is not implemented for MyBLAS::Vector<DataType>
template <typename DataType>
MyBLAS::Vector<DataType> operator*(MyBLAS::Vector<DataType> const& a, LazyMatrix<DataType> const& b) = delete;
}

#endif // NE591_008_LAZYMATRIX_H
