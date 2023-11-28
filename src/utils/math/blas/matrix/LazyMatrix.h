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
#include <memory>

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
    explicit LazyMatrix(size_t rows = 0, size_t cols = 0, Generator generator = nullptr)
        : rows_(rows), cols_(cols), generator_(std::move(generator)) {
        ResourceMonitor<LazyMatrix<DataType>>::registerInstance(this);
    }

    /**
     * @brief Constructor for the LazyMatrix class that accepts a Matrix object.
     *
     * @param matrix The Matrix object to initialize the LazyMatrix with.
     * The generator function is set to access the elements of the given matrix.
     */
    explicit LazyMatrix(const MyBLAS::Matrix<DataType>& matrix)
        : rows_(matrix.getRows()), cols_(matrix.getCols()), matrix_(std::make_shared<MyBLAS::Matrix<DataType>>(matrix)) {
        generator_ = [matrix](size_t i, size_t j) { return matrix(i, j); };
        ResourceMonitor<LazyMatrix<DataType>>::registerInstance(this);
    }

    /**
     * @brief Constructor for the LazyMatrix class that accepts a non-const reference to a Matrix object.
     *
     * @param matrix The Matrix object to initialize the LazyMatrix with.
     * The generator function is set to access the elements of the given matrix.
     */
    explicit LazyMatrix(MyBLAS::Matrix<DataType>& matrix)
        : rows_(matrix.getRows()), cols_(matrix.getCols()), matrix_(std::make_shared<MyBLAS::Matrix<DataType>>(matrix)) {
        generator_ = [matrix](size_t i, size_t j) { return matrix(i, j); };
        ResourceMonitor<LazyMatrix<DataType>>::registerInstance(this);
    }

    [[nodiscard]] virtual /**
     * @brief Returns the number of bytes allocated by the LazyMatrix.
     *
     * @param actual If true, calculates the actual allocated bytes, otherwise returns the size of the LazyMatrix object.
     * @return The number of bytes allocated by the LazyMatrix.
     */
    size_t getAllocatedBytes(bool actual = false) const {
        return sizeof(*this);
    }

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
     * @brief Destructor for the LazyMatrix class.
     */
    virtual ~LazyMatrix() {
        ResourceMonitor<LazyMatrix<DataType>>::unregisterInstance(this);
    }

    /**
     * @brief Copy constructor for the LazyMatrix class.
     *
     * @param other The LazyMatrix object to be copied.
     */
    LazyMatrix(const LazyMatrix& other) : rows_(other.rows_), cols_(other.cols_), generator_(other.generator_) {
        ResourceMonitor<LazyMatrix<DataType>>::registerInstance(this);
    }

    /**
     * @brief Move constructor for the LazyMatrix class.
     *
     * @param other The LazyMatrix object to be copied.
     */
    LazyMatrix(const LazyMatrix&& other) noexcept
        : rows_(other.rows_), cols_(other.cols_), generator_(std::move(other.generator_)), matrix_(std::move(other.matrix_)) {
        other.rows_ = 0;
        other.cols_ = 0;
        other.generator_ = nullptr;
        other.matrix_ = nullptr;
        ResourceMonitor<LazyMatrix<DataType>>::registerInstance(this);
    }

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
            matrix_ = other.matrix_;
            ResourceMonitor<LazyMatrix<DataType>>::registerInstance(this);
        }
        return *this;
    }

    /**
     * @brief Overload of the multiplication operator for multiplying a MyBLAS::Matrix and a LazyMatrix.
     * @details This function performs matrix-matrix multiplication between a MyBLAS::Matrix and a LazyMatrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the MyBLAS::Matrix.
     * @param b A constant reference to the LazyMatrix.
     * @return A LazyMatrix that represents the result of the multiplication.
     */
    friend LazyMatrix operator*(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        return ContainerExpression<MyBLAS::Matrix, LazyMatrix, DataType>::multiplyMatrixMatrixType2(a, b);
    }

    /**
     * @brief Overload of the multiplication operator for multiplying a LazyMatrix and a MyBLAS::Matrix.
     * @details This function performs matrix-matrix multiplication between a LazyMatrix and a MyBLAS::Matrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the LazyMatrix.
     * @param b A constant reference to the MyBLAS::Matrix.
     * @return A LazyMatrix that represents the result of the multiplication.
     */
    friend LazyMatrix operator*(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        return ContainerExpression<LazyMatrix, MyBLAS::Matrix, DataType>::multiplyMatrixMatrixType1(a, b);
    }

    /**
     * @brief Overload of the multiplication operator for multiplying two LazyMatrix objects.
     * @details This function performs matrix-matrix multiplication between two LazyMatrix objects.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the first LazyMatrix.
     * @param b A constant reference to the second LazyMatrix.
     * @return A LazyMatrix that represents the result of the multiplication.
     */
    friend LazyMatrix operator*(LazyMatrix const& a, LazyMatrix const& b) {
        return ContainerExpression<LazyMatrix, LazyMatrix, DataType>::multiplyMatrixMatrixType2(a, b);
    }

    /**
     * @brief Overload of the multiplication operator for multiplying a LazyMatrix and a MyBLAS::Vector.
     * @details This function performs matrix-vector multiplication between a LazyMatrix and a MyBLAS::Vector.
     * @tparam DataType The type of data contained in the matrix and the vector.
     * @param a A constant reference to the LazyMatrix.
     * @param b A constant reference to the MyBLAS::Vector.
     * @return A MyBLAS::Vector that represents the result of the multiplication.
     */
    friend MyBLAS::Vector<DataType> operator*(LazyMatrix const& a, MyBLAS::Vector<DataType> const& b) {
        return MatrixVectorExpression<LazyMatrix, MyBLAS::Vector, DataType>::multiplyMatrixVector(a, b);
    }

    /**
     * @brief Overload of the multiplication operator for multiplying a LazyMatrix and a LazyVector.
     * @details This function performs matrix-vector multiplication between a LazyMatrix and a LazyVector.
     * @tparam DataType The type of data contained in the matrix and the vector.
     * @param a A constant reference to the LazyMatrix.
     * @param b A constant reference to the LazyVector.
     * @return A LazyVector that represents the result of the multiplication.
     */
    friend LazyVector<DataType> operator*(LazyMatrix const& a, LazyVector<DataType> const& b) {
        return MatrixVectorExpression<LazyMatrix, LazyVector, DataType>::multiplyMatrixVector(a, b);
    }

    /**
     * @brief Overload of the addition operator for adding a MyBLAS::Matrix and a LazyMatrix.
     * @details This function performs element-wise addition between a MyBLAS::Matrix and a LazyMatrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the MyBLAS::Matrix.
     * @param b A constant reference to the LazyMatrix.
     * @return A LazyMatrix that represents the result of the addition.
     */
    friend LazyMatrix operator+(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x + y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the addition operator for adding a LazyMatrix and a MyBLAS::Matrix.
     * @details This function performs element-wise addition between a LazyMatrix and a MyBLAS::Matrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the LazyMatrix.
     * @param b A constant reference to the MyBLAS::Matrix.
     * @return A LazyMatrix that represents the result of the addition.
     */
    friend LazyMatrix operator+(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y + x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }

    /**
     * @brief Overload of the addition operator for adding two LazyMatrix objects.
     * @details This function performs element-wise addition between two LazyMatrix objects.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the first LazyMatrix.
     * @param b A constant reference to the second LazyMatrix.
     * @return A LazyMatrix that represents the result of the addition.
     */
    friend LazyMatrix operator+(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x + y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the subtraction operator for subtracting a LazyMatrix from a MyBLAS::Matrix.
     * @details This function performs element-wise subtraction between a MyBLAS::Matrix and a LazyMatrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the MyBLAS::Matrix.
     * @param b A constant reference to the LazyMatrix.
     * @return A LazyMatrix that represents the result of the subtraction.
     */
    friend LazyMatrix operator-(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x - y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the subtraction operator for subtracting a MyBLAS::Matrix from a LazyMatrix.
     * @details This function performs element-wise subtraction between a LazyMatrix and a MyBLAS::Matrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the LazyMatrix.
     * @param b A constant reference to the MyBLAS::Matrix.
     * @return A LazyMatrix that represents the result of the subtraction.
     */
    friend LazyMatrix operator-(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y - x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }

    /**
     * @brief Overload of the subtraction operator for subtracting one LazyMatrix from another.
     * @details This function performs element-wise subtraction between two LazyMatrix objects.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the first LazyMatrix.
     * @param b A constant reference to the second LazyMatrix.
     * @return A LazyMatrix that represents the result of the subtraction.
     */
    friend LazyMatrix operator-(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x - y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the equality operator for comparing a MyBLAS::Matrix and a LazyMatrix.
     * @details This function performs element-wise comparison between a MyBLAS::Matrix and a LazyMatrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the MyBLAS::Matrix.
     * @param b A constant reference to the LazyMatrix.
     * @return A LazyMatrix that represents the result of the comparison.
     */
    friend LazyMatrix operator==(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x == y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the equality operator for comparing a LazyMatrix and a MyBLAS::Matrix.
     * @details This function performs element-wise comparison between a LazyMatrix and a MyBLAS::Matrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the LazyMatrix.
     * @param b A constant reference to the MyBLAS::Matrix.
     * @return A LazyMatrix that represents the result of the comparison.
     */
    friend LazyMatrix operator==(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y == x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }

    /**
     * @brief Overload of the equality operator for comparing two LazyMatrix objects.
     * @details This function performs element-wise comparison between two LazyMatrix objects.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the first LazyMatrix.
     * @param b A constant reference to the second LazyMatrix.
     * @return A LazyMatrix that represents the result of the comparison.
     */
    friend LazyMatrix operator==(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x == y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the inequality operator for comparing a MyBLAS::Matrix and a LazyMatrix.
     * @details This function performs element-wise comparison between a MyBLAS::Matrix and a LazyMatrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the MyBLAS::Matrix.
     * @param b A constant reference to the LazyMatrix.
     * @return A LazyMatrix that represents the result of the comparison.
     */
    friend LazyMatrix operator!=(MyBLAS::Matrix<DataType> const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x != y; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the inequality operator for comparing a LazyMatrix and a MyBLAS::Matrix.
     * @details This function performs element-wise comparison between a LazyMatrix and a MyBLAS::Matrix.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the LazyMatrix.
     * @param b A constant reference to the MyBLAS::Matrix.
     * @return A LazyMatrix that represents the result of the comparison.
     */
    friend LazyMatrix operator!=(LazyMatrix const& a, MyBLAS::Matrix<DataType> const& b) {
        auto op = [](DataType const& x, DataType const& y) { return y != x; };
        return ElementwiseExpression<DataType, MyBLAS::Matrix<DataType>, LazyMatrix, decltype(op)>::matrix(b, a, op);
    }

    /**
     * @brief Overload of the inequality operator for comparing two LazyMatrix objects.
     * @details This function performs element-wise comparison between two LazyMatrix objects.
     * @tparam DataType The type of data contained in the matrices.
     * @param a A constant reference to the first LazyMatrix.
     * @param b A constant reference to the second LazyMatrix.
     * @return A LazyMatrix that represents the result of the comparison.
     */
    friend LazyMatrix operator!=(LazyMatrix const& a, LazyMatrix const& b) {
        auto op = [](DataType const& x, DataType const& y) { return x != y; };
        return ElementwiseExpression<DataType, LazyMatrix, LazyMatrix, decltype(op)>::matrix(a, b, op);
    }

    /**
     * @brief Overload of the addition operator for adding a scalar to a LazyMatrix.
     * @details This function performs element-wise addition between a LazyMatrix and a scalar.
     * @tparam DataType The type of data contained in the matrix and the scalar.
     * @param scalar The scalar to be added to the matrix.
     * @return A LazyMatrix that represents the result of the addition.
     */
    LazyMatrix operator+(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) + scalar; });
    }

    /**
     * @brief Overload of the subtraction operator for subtracting a scalar from a LazyMatrix.
     * @details This function performs element-wise subtraction between a LazyMatrix and a scalar.
     * @tparam DataType The type of data contained in the matrix and the scalar.
     * @param scalar The scalar to be subtracted from the matrix.
     * @return A LazyMatrix that represents the result of the subtraction.
     */
    LazyMatrix operator-(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) - scalar; });
    }

    /**
     * @brief Overload of the multiplication operator for multiplying a LazyMatrix by a scalar.
     * @details This function performs element-wise multiplication between a LazyMatrix and a scalar.
     * @tparam DataType The type of data contained in the matrix and the scalar.
     * @param scalar The scalar to multiply the matrix by.
     * @return A LazyMatrix that represents the result of the multiplication.
     */
    LazyMatrix operator*(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) * scalar; });
    }

    /**
     * @brief Overload of the division operator for dividing a LazyMatrix by a scalar.
     * @details This function performs element-wise division between a LazyMatrix and a scalar.
     * @tparam DataType The type of data contained in the matrix and the scalar.
     * @param scalar The scalar to divide the matrix by.
     * @return A LazyMatrix that represents the result of the division.
     */
    LazyMatrix operator/(DataType scalar) const {
        return LazyMatrix(getRows(), getCols(), [this, scalar](size_t i, size_t j) { return (*this)(i, j) / scalar; });
    }

    /**
     * @brief Overload of the unary negation operator for a LazyMatrix.
     * @details This function performs element-wise negation on a LazyMatrix.
     * @return A LazyMatrix that represents the result of the negation.
     */
    LazyMatrix operator-() const {
        return LazyMatrix(getRows(), getCols(), [this](size_t i, size_t j) { return -(*this)(i, j); });
    }

    /**
     * @brief Overload of the addition assignment operator for adding a LazyMatrix to the current matrix.
     * @details This function performs element-wise addition between the current matrix and another LazyMatrix.
     * @param b A constant reference to the LazyMatrix to be added.
     * @return A reference to the current matrix which now represents the result of the addition.
     */
    LazyMatrix& operator+=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) + b(i, j); };
        return *this;
    }

    /**
     * @brief Overload of the subtraction assignment operator for subtracting a LazyMatrix from the current matrix.
     * @details This function performs element-wise subtraction between the current matrix and another LazyMatrix.
     * @param b A constant reference to the LazyMatrix to be subtracted.
     * @return A reference to the current matrix which now represents the result of the subtraction.
     */
     LazyMatrix& operator-=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) - b(i, j); };
        return *this;
    }

    /**
     * @brief Overload of the multiplication assignment operator for multiplying a LazyMatrix with the current matrix.
     * @details This function performs element-wise multiplication between the current matrix and another LazyMatrix.
     * @param b A constant reference to the LazyMatrix to be multiplied.
     * @return A reference to the current matrix which now represents the result of the multiplication.
     */
    LazyMatrix& operator*=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) * b(i, j); };
        return *this;
    }

    /**
     * @brief Overload of the division assignment operator for dividing the current matrix by a LazyMatrix.
     * @details This function performs element-wise division between the current matrix and another LazyMatrix.
     * @param b A constant reference to the LazyMatrix to be divided by.
     * @return A reference to the current matrix which now represents the result of the division.
     */
    LazyMatrix& operator/=(LazyMatrix const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i, size_t j) { return old_gen(i, j) / b(i, j); };
        return *this;
    }

    /**
     * @brief Overload of the addition assignment operator for adding a scalar to the current matrix.
     * @details This function performs element-wise addition between the current matrix and a scalar.
     * @param scalar The scalar to be added.
     * @return A reference to the current matrix which now represents the result of the addition.
     */
    LazyMatrix& operator+=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) + scalar; };
        return *this;
    }

    /**
     * @brief Overload of the subtraction assignment operator for subtracting a scalar from the current matrix.
     * @details This function performs element-wise subtraction between the current matrix and a scalar.
     * @param scalar The scalar to be subtracted.
     * @return A reference to the current matrix which now represents the result of the subtraction.
     */
    LazyMatrix& operator-=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) - scalar; };
        return *this;
    }

    /**
     * @brief Overload of the multiplication assignment operator for multiplying the current matrix by a scalar.
     * @details This function performs element-wise multiplication between the current matrix and a scalar.
     * @param scalar The scalar to be multiplied.
     * @return A reference to the current matrix which now represents the result of the multiplication.
     */
    LazyMatrix& operator*=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) * scalar; };
        return *this;
    }

    /**
     * @brief Overload of the division assignment operator for dividing the current matrix by a scalar.
     * @details This function performs element-wise division between the current matrix and a scalar.
     * @param scalar The scalar to be divided by.
     * @return A reference to the current matrix which now represents the result of the division.
     */
    LazyMatrix& operator/=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i, size_t j) { return old_gen(i, j) / scalar; };
        return *this;
    }

    /**
     * @brief Function to compare two LazyMatrix objects for element-wise equality.
     * @details This function generates a new LazyMatrix of boolean values, where each element represents the equality of the corresponding elements in the input matrices.
     * @param a A constant reference to the first LazyMatrix.
     * @param b A constant reference to the second LazyMatrix.
     * @return A LazyMatrix of boolean values, where each element represents the equality of the corresponding elements in the input matrices.
     */
    friend LazyMatrix<bool> equal(LazyMatrix const& a, LazyMatrix const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x == y; };
        ElementwiseExpression<bool, LazyMatrix, LazyMatrix, decltype(binary_op)> expr(a, b, binary_op);
        return LazyMatrix<bool>(a.getRows(), a.getCols(), [expr](size_t i, size_t j) { return expr(i, j); });
    }

    /**
     * @brief Function to compare two LazyMatrix objects for equality.
     * @details This function checks if all elements in the two input matrices are equal.
     * @param a A constant reference to the first LazyMatrix.
     * @param b A constant reference to the second LazyMatrix.
     * @return A boolean value indicating whether all elements in the two input matrices are equal.
     */
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

    /**
     * @class Proxy
     * @brief A helper class to provide a proxy for non-const access to a row of the LazyMatrix.
     * @details This class is used to overload the [] operator for non-const access to a row of the LazyMatrix.
     */
    class Proxy {
      public:
        /**
         * @brief Constructor for the Proxy class.
         * @details This constructor initializes a Proxy object for non-const access to a row of the LazyMatrix.
         * @param matrix A reference to the LazyMatrix object.
         * @param row The index of the row to be accessed.
         */
        Proxy(LazyMatrix& matrix, size_t row) : matrix_(matrix), row_(row) {}

        /**
         * @brief Overload of the [] operator for non-const access to a column of the LazyMatrix.
         * @param col The index of the column to access.
         * @return The value at the specified row and column in the LazyMatrix.
         */
        DataType operator[](size_t col) const {
            return matrix_(row_, col);
        }

      private:
        /**
         * @brief A reference to the LazyMatrix object.
         */
        LazyMatrix& matrix_;

        /**
         * @brief The index of the row to be accessed in the LazyMatrix.
         */
        size_t row_;
    };

    /**
     * @class ConstProxy
     * @brief A helper class to provide a proxy for const access to a row of the LazyMatrix.
     * @details This class is used to overload the [] operator for const access to a row of the LazyMatrix.
     */
    class ConstProxy {
      public:
        /**
         * @brief Constructor for the ConstProxy class.
         * @details This constructor initializes a ConstProxy object for const access to a row of the LazyMatrix.
         * @param matrix A constant reference to the LazyMatrix object.
         * @param row The index of the row to be accessed.
         */
        ConstProxy(const LazyMatrix& matrix, size_t row) : matrix_(matrix), row_(row) {}

        /**
         * @brief Overload of the [] operator for const access to a column of the LazyMatrix.
         * @param col The index of the column to access.
         * @return The value at the specified row and column in the LazyMatrix.
         */
        DataType operator[](size_t col) const {
            return matrix_(row_, col);
        }

      private:
        /**
         * @brief A constant reference to the LazyMatrix object.
         */
        const LazyMatrix& matrix_;

        /**
         * @brief The index of the row to be accessed in the LazyMatrix.
         */
        size_t row_;
    };

    /**
     * @brief Overload of the [] operator for non-const access to a row of the LazyMatrix.
     * @param row The index of the row to access.
     * @return A Proxy object representing the row at the given index.
     */
    Proxy operator[](size_t row) {
        return Proxy(*this, row);
    }

    /**
     * @brief Overload of the [] operator for const access to a row of the LazyMatrix.
     * @param row The index of the row to access.
     * @return A ConstProxy object representing the row at the given index.
     */
    ConstProxy operator[](size_t row) const {
        return ConstProxy(*this, row);
    }

  private:
    /**
     * @brief The number of rows in the LazyMatrix.
     */
    size_t rows_{};

    /**
     * @brief The number of columns in the LazyMatrix.
     */
    size_t cols_{};

    /**
     * @brief The generator function that computes the values of the LazyMatrix.
     */
    Generator generator_;

    /**
     * @brief A shared pointer to the memory backed matrix
     */
    std::shared_ptr<MyBLAS::Matrix<DataType>> matrix_;

  protected:
    /**
     * @brief Setter for the number of rows in the LazyMatrix.
     * @param rows The new number of rows.
     */
    void setRows(size_t rows) { rows_ = rows; }

    /**
     * @brief Setter for the number of columns in the LazyMatrix.
     * @param cols The new number of columns.
     */
    void setCols(size_t cols) { cols_ = cols; }

    /**
     * @brief Setter for the generator function of the LazyMatrix.
     * @param generator The new generator function.
     */
    void setGenerator(const Generator &generator) { generator_ = generator; }
};

/**
 * @brief This function template is a placeholder for vector-matrix multiplication in the MyBLAS library.
 * @details This function is currently not implemented and is marked for deletion.
 * It is intended to perform multiplication of a vector and a lazy matrix.
 * @tparam DataType The type of data contained in the vector and the lazy matrix.
 * @param a A constant reference to the vector.
 * @param b A constant reference to the lazy matrix.
 * @return This function does not return a value as it is marked for deletion.
 * @note This function is marked as deleted and will cause a compile error if used.
 */
template <typename DataType>
MyBLAS::Vector<DataType> operator*(MyBLAS::Vector<DataType> const& a, LazyMatrix<DataType> const& b) = delete;
}

#endif // NE591_008_LAZYMATRIX_H
