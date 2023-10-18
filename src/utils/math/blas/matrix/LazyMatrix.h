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

#include "Matrix.h"
#include "MatrixExpression.h"
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

    // Define the addition operation.
    template <typename MatrixType1, typename MatrixType2>
    static LazyMatrix addMatrices(const MatrixType1& a, const MatrixType2& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x + y; };
        MatrixExpression<DataType, MatrixType1, MatrixType2, decltype(binary_op)> expr(a, b, binary_op);
        return LazyMatrix(a.getRows(), a.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }

    template <typename MatrixType>
    friend LazyMatrix operator+(LazyMatrix const& a, MatrixType const& b) {
        return addMatrices(a, b);
    }

    template <typename MatrixType>
    friend LazyMatrix operator+(MatrixType const& a, LazyMatrix const& b) {
        return addMatrices(a, b);
    }

    friend LazyMatrix operator+(LazyMatrix const& a, LazyMatrix const& b) {
        return addMatrices(a, b);
    }

    // Define the subtraction operation.
    template <typename MatrixType1, typename MatrixType2>
    static LazyMatrix subtractMatrices(const MatrixType1& a, const MatrixType2& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x - y; };
        MatrixExpression<DataType, MatrixType1, MatrixType2, decltype(binary_op)> expr(a, b, binary_op);
        return LazyMatrix(a.getRows(), a.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }

    template <typename MatrixType>
    friend LazyMatrix operator-(LazyMatrix const& a, MatrixType const& b) {
        return subtractMatrices(a, b);
    }

    template <typename MatrixType>
    friend LazyMatrix operator-(MatrixType const& a, LazyMatrix const& b) {
        return subtractMatrices(a, b);
    }

    friend LazyMatrix operator-(LazyMatrix const& a, LazyMatrix const& b) {
        return subtractMatrices(a, b);
    }

    template <typename MatrixType1, typename MatrixType2>
    static LazyMatrix multiplyMatrices(const MatrixType1& a, const MatrixType2& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x * y; };
        MatrixExpression<DataType, MatrixType1, MatrixType2, decltype(binary_op)> expr(a, b, binary_op);
        return LazyMatrix(a.getRows(), a.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }

    template <typename MatrixType>
    friend LazyMatrix operator*(LazyMatrix const& a, MatrixType const& b) {
        return multiplyMatrices(a, b);
    }

    template <typename MatrixType>
    friend LazyMatrix operator*(MatrixType const& a, LazyMatrix const& b) {
        return multiplyMatrices(a, b);
    }

    friend LazyMatrix operator*(LazyMatrix const& a, LazyMatrix const& b) {
        return multiplyMatrices(a, b);
    }

    // Define the scalar addition operation.
    LazyMatrix operator+(DataType scalar) const {
        return LazyMatrix(rows_, cols_, [this, scalar](size_t i, size_t j) { return (*this)(i, j) + scalar; });
    }

    // Define the scalar subtraction operation.
    LazyMatrix operator-(DataType scalar) const {
        return LazyMatrix(rows_, cols_, [this, scalar](size_t i, size_t j) { return (*this)(i, j) - scalar; });
    }

    // Define the scalar multiplication operation.
    LazyMatrix operator*(DataType scalar) const {
        return LazyMatrix(rows_, cols_, [this, scalar](size_t i, size_t j) { return (*this)(i, j) * scalar; });
    }

    // Define the scalar division operation.
    LazyMatrix operator/(DataType scalar) const {
        return LazyMatrix(rows_, cols_, [this, scalar](size_t i, size_t j) { return (*this)(i, j) / scalar; });
    }

    // Define the unary negation operation.
    LazyMatrix operator-() const {
        return LazyMatrix(rows_, cols_, [this](size_t i, size_t j) { return -(*this)(i, j); });
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
        MatrixExpression<bool, LazyMatrix, LazyMatrix, decltype(binary_op)> expr(a, b, binary_op);
        return LazyMatrix<bool>(a.getRows(), a.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }

    // Define the equality comparison operation.
    friend bool operator==(LazyMatrix const& a, LazyMatrix const& b) {
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

    // Define the inequality comparison operation.
    friend bool operator!=(LazyMatrix const& a, LazyMatrix const& b) {
        return !(a == b);
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
