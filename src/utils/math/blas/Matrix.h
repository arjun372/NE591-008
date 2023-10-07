/**
 * @file Matrix.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains my BLAS::Matrix declarations.
 */

#ifndef NE591_008_MATRIX_H
#define NE591_008_MATRIX_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "math/blas/Vector.h"

namespace MyBLAS {

template <typename T> class Matrix;
template <typename T> class Vector;

/**
 * @class Matrix
 * @brief Class representing a matrix of T type values.
 */
template <typename T> class Matrix {

  protected:
    std::vector<std::vector<T>> data; ///< 2D vector representing the matrix data.
    size_t rows, cols;                ///< Number of rows and columns in the matrix.

  public:
    /**
     * @brief Default constructor. Initializes an empty matrix.
     */
    Matrix() : rows(0), cols(0) {}

    /**
     * @brief Constructor that initializes the matrix with a given 2D vector.
     * @param _data 2D vector to initialize the matrix with.
     */
    explicit Matrix(std::vector<std::vector<T>> &_data) : data(_data), rows(_data.size()), cols(_data.size()) {}

    /**
     * @brief Constructor that initializes the matrix with a given 2D vector.
     * @param _data 2D vector to initialize the matrix with.
     */
    explicit Matrix(std::vector<std::vector<T>> _data) : data(_data), rows(_data.size()), cols(_data.size()) {}

    /**
     * @brief Parameterized constructor that initializes the matrix with a given size and initial value.
     * @param _rows Number of rows in the matrix.
     * @param _cols Number of columns in the matrix.
     * @param _initial Initial value for all elements in the matrix.
     */
    Matrix(size_t _rows, size_t _cols, const T _initial = 0)
        : data(_rows, std::vector<T>(_cols, _initial)), rows(_rows), cols(_cols) {}

    /**
     * @brief Constructor that initializes the matrix with a given initializer list.
     * @param initList Initializer list to initialize the matrix with.
     */
    Matrix(std::initializer_list<std::initializer_list<T>> initList) {
        rows = initList.size();
        cols = initList.begin()->size();
        data.resize(rows);
        size_t i = 0;
        for (const auto &row : initList) {
            data[i].resize(cols);
            size_t j = 0;
            for (const auto &elem : row) {
                data[i][j] = elem;
                ++j;
            }
            ++i;
        }
    }

    /**
     * @brief Overloaded operator[] to access individual rows of the matrix.
     * @param rowNum Index of the row to access.
     * @return Reference to the row at the given index.
     */
    std::vector<T> &operator[](const size_t rowNum) { return data[rowNum]; }

    /**
     * @brief Overloaded operator[] to access individual rows of the matrix (const version).
     * @param rowNum Index of the row to access.
     * @return Const reference to the row at the given index.
     */
    const std::vector<T> &operator[](const size_t rowNum) const { return data[rowNum]; }

    /**
     * @brief Getter for the matrix data.
     * @return Const reference to the matrix data.
     */
    [[nodiscard]] const std::vector<std::vector<T>> &getData() const { return data; }

    /**
     * @brief Adds a new row to the end of the matrix.
     * @param row Vector representing the new row to be added.
     */
    void push_back(const std::vector<T> &row) {
        if (data.empty()) {
            cols = row.size();
        } else if (cols != row.size()) {
            throw std::invalid_argument("Error: Row size does not match the matrix column size.");
        }
        data.push_back(row);
        ++rows;
    }

    /**
     * @brief Getter for the number of rows in the matrix.
     * @return Number of rows in the matrix.
     */
    [[nodiscard]] size_t getRows() const {
        if (rows != data.size()) {
            throw std::invalid_argument("Error: Matrix row size does not match the allocated matrix rows.");
        }
        return rows;
    }

    /**
     * @brief Getter for the number of columns in the matrix.
     * @return Number of columns in the matrix.
     */
    [[nodiscard]] size_t getCols() const {
        size_t colSize;
        try {
            colSize = data[0].size();
        } catch (...) {
            colSize = 0;
        }
        if (cols != colSize) {
            throw std::invalid_argument("Error: Matrix column size does not match the allocated matrix columns.");
        }
        return cols;
    }

    /**
     * @brief Static function to get the identity matrix of a given size.
     * @param size Size of the identity matrix.
     * @return Identity matrix of the given size.
     */
    static Matrix eye(size_t size) {
        Matrix eye(size, size, 0);
        for (size_t i = 0; i < size; ++i) {
            eye[i][i] = 1;
        }
        return eye;
    }

    /**
     * @brief Overloaded operator+ to add two matrices.
     * @param rhs Matrix to add to the current matrix.
     * @return Resultant matrix after addition.
     */
    Matrix operator+(const Matrix &rhs) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            throw std::exception();
        }
        Matrix result(rows, cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = this->data[i][j] + rhs.data[i][j];
            }
        }
        return result;
    }

    /**
     * @brief Overloaded operator- to subtract two matrices.
     * @param rhs Matrix to subtract from the current matrix.
     * @return Resultant matrix after subtraction.
     */
    Matrix operator-(const Matrix &rhs) const {
        if (rows != rhs.rows || cols != rhs.cols) {
            throw std::exception();
        }
        Matrix result(rows, cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = this->data[i][j] - rhs.data[i][j];
            }
        }
        return result;
    }

    /**
     * @brief Overloaded operator* to multiply two matrices.
     * @param rhs Matrix to multiply with the current matrix.
     * @return Resultant matrix after multiplication.
     */
    Matrix operator*(const Matrix &rhs) const {
        if (cols != rhs.rows) {
            throw std::exception();
        }
        Matrix result(rows, rhs.cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < rhs.cols; ++j) {
                for (size_t k = 0; k < cols; ++k) {
                    result[i][j] += this->data[i][k] * rhs.data[k][j];
                }
            }
        }
        return result;
    }

    /**
     * @brief Overloaded operator* to multiply a matrix with a vector.
     * @param rhs Vector to multiply with the current matrix.
     * @return Resultant vector after multiplication.
     */
    Vector<T> operator*(const Vector<T> &rhs) const {
        if (cols != rhs.size()) {
            throw std::exception();
        }
        Vector<T> result(rows, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i] += this->data[i][j] * rhs[j];
            }
        }
        return result;
    }

    /**
     * @brief Sets a submatrix within the current matrix.
     * @param rowStart Starting row index for the submatrix.
     * @param colStart Starting column index for the submatrix.
     * @param subMatrix The submatrix to set.
     */
    void setSubMatrix(size_t rowStart, size_t colStart, const Matrix<T> &subMatrix) {
        size_t subRows = subMatrix.getRows();
        size_t subCols = subMatrix.getCols();

        if (rowStart + subRows > rows || colStart + subCols > cols) {
            throw std::exception();
        }

        for (size_t i = 0; i < subRows; ++i) {
            for (size_t j = 0; j < subCols; ++j) {
                data[rowStart + i][colStart + j] = subMatrix[i][j];
            }
        }
    }

    /**
     * @brief Extracts a submatrix from the current matrix.
     * @param rowStart Starting row index for the submatrix.
     * @param colStart Starting column index for the submatrix.
     * @param subRows Number of rows in the submatrix.
     * @param subCols Number of columns in the submatrix.
     * @return The extracted submatrix.
     */
    Matrix<T> subMatrix(size_t rowStart, size_t colStart, size_t subRows, size_t subCols) const {
        if (rowStart + subRows > rows || colStart + subCols > cols) {
            throw std::exception();
        }

        Matrix<T> subMatrix(subRows, subCols, 0);
        for (size_t i = 0; i < subRows; ++i) {
            for (size_t j = 0; j < subCols; ++j) {
                subMatrix[i][j] = data[rowStart + i][colStart + j];
            }
        }

        return subMatrix;
    }

    /**
     * @brief Overloaded operator* to multiply the matrix by a scalar.
     * @param scalar Scalar to multiply the matrix with.
     * @return Resultant matrix after multiplication.
     */
    Matrix operator*(const T &scalar) const {
        Matrix result(rows, cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = this->data[i][j] * scalar;
            }
        }
        return result;
    }

    /**
     * @brief Overloaded operator/ to divide the matrix by a scalar.
     * @param scalar Scalar to divide the matrix by.
     * @return Resultant matrix after division.
     */
    Matrix operator/(const T &scalar) const {
        const T divisor = scalar == 0 ? NAN : scalar;
        Matrix result(rows, cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = this->data[i][j] / divisor;
            }
        }
        return result;
    }

    /**
     * @brief Overloaded operator+ to add a scalar to the matrix.
     * @param scalar Scalar to add to the matrix.
     * @return Resultant matrix after addition.
     */
    Matrix operator+(const T &scalar) const {
        Matrix result(rows, cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = this->data[i][j] + scalar;
            }
        }
        return result;
    }

    /**
     * @brief Overloaded operator- to subtract a scalar from the matrix.
     * @param scalar Scalar to subtract from the matrix.
     * @return Resultant matrix after subtraction.
     */
    Matrix operator-(const T &scalar) const {
        Matrix result(rows, cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = this->data[i][j] - scalar;
            }
        }
        return result;
    }

    /**
     * @brief Swaps two rows in the matrix.
     * @param row1 Index of the first row to swap.
     * @param row2 Index of the second row to swap.
     */
    void swapRows(size_t row1, size_t row2) {
        if (row1 >= rows || row2 >= rows) {
            return;
        }
        std::swap(data[row1], data[row2]);
    }

    /**
     * @brief Overloaded operator<< to print the matrix to an output stream.
     * @param os Output stream to print the matrix to.
     * @param m Matrix to print.
     * @return Reference to the output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
        const auto width = static_cast<int>(std::cout.precision() + static_cast<std::streamsize>(10));
        for (size_t i = 0; i < m.getRows(); ++i) {
            for (size_t j = 0; j < m.getCols(); ++j) {
                os << std::setw(width) << std::setfill(' ') << std::scientific << static_cast<long double>(m[i][j]);
            }
            os << '\n';
        }
        return os;
    }
};

/**
 * @brief Perform forward substitution
 *
 * This function performs forward substitution, which is used in solving a system of linear equations
 * after the system matrix has been decomposed into a lower triangular matrix (L) and an upper triangular matrix (U).
 * The forward substitution algorithm iterates through each row of the lower triangular matrix (L) and computes
 * the corresponding element in the intermediate result vector (y) by subtracting the sum of the product of the
 * current row elements and the corresponding elements in the intermediate result vector (y) from the corresponding
 * element in the input vector (b).
 *
 * @tparam T Using templates for data type consistency in computation.
 * @param L A lower triangular matrix.
 * @param b A vector in the system of linear equations Ax = b.
 * @return The result vector after performing forward substitution.
 */
template <typename T>
static MyBLAS::Vector<T> forwardSubstitution(const MyBLAS::Matrix<T> &L, const MyBLAS::Vector<T> &b) {
    const auto n = b.size();
    auto y = MyBLAS::Vector<T>(n);
    for (size_t row = 0; row < n; row++) {
        T sum = 0;
        for (size_t col = 0; col < row; col++) {
            sum += L[row][col] * y[col];
        }
        y[row] = (b[row] - sum);
    }
    return y;
}

/**
 * @brief Perform backward substitution
 *
 * This function performs backward substitution, which is used in solving a system of linear equations
 * after the system matrix has been decomposed into a lower triangular matrix (L) and an upper triangular matrix (U),
 * and forward substitution has been performed. The backward substitution algorithm iterates through each row of the
 * upper triangular matrix (U) in reverse order and computes the corresponding element in the solution vector (x) by
 * subtracting the sum of the product of the current row elements and the corresponding elements in the solution vector
 * (x) from the corresponding element in the intermediate result vector (y) and then dividing by the diagonal element of
 * the current row in the upper triangular matrix (U).
 *
 * @tparam T Using templates for data type consistency in computation.
 * @param U An upper triangular matrix.
 * @param y The result vector after performing forward substitution.
 * @return The solution vector after performing backward substitution.
 */
template <typename T>
static MyBLAS::Vector<T> backwardSubstitution(const MyBLAS::Matrix<T> &U, const MyBLAS::Vector<T> &y) {
    const auto n = y.size();
    auto x = MyBLAS::Vector<T>(n, static_cast<T>(0));
    for (int64_t i = n - 1; i >= 0; i--) {
        T sum = 0;
        for (size_t j = i + 1; j < n; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }
    return x;
}

/**
 * @brief Casts the elements of a given matrix to a specified type.
 *
 * This function creates a new matrix with the same dimensions as the input matrix and fills it with the
 * elements of the input matrix cast to the specified type.
 *
 * @tparam T The data type to cast the elements to.
 * @tparam T2 The original data type of the elements in the input matrix.
 * @param input The input matrix whose elements are to be cast.
 * @return A new matrix with the elements of the input matrix cast to the specified type.
 */
template <typename T, typename T2> static MyBLAS::Matrix<T> cast(const MyBLAS::Matrix<T2> &input) {
    MyBLAS::Matrix<T> output = Matrix<T>(input.getRows(), input.getCols(), 0);

    for (size_t row = 0; row < output.getRows(); row++) {
        for (size_t col = 0; col < output.getCols(); col++) {
            output[row][col] = static_cast<T>(input[row][col]);
        }
    }
    return output;
}

/**
 * @brief Casts the elements of a given vector to a specified type.
 *
 * This function creates a new vector with the same size as the input vector and fills it with the
 * elements of the input vector cast to the specified type.
 *
 * @tparam T The data type to cast the elements to.
 * @tparam T2 The original data type of the elements in the input vector.
 * @param input The input vector whose elements are to be cast.
 * @return A new vector with the elements of the input vector cast to the specified type.
 */
template <typename T, typename T2> static MyBLAS::Vector<T> cast(const MyBLAS::Vector<T2> &input) {
    MyBLAS::Vector<T> output = Vector<T>(input.size(), 0);
    for (size_t idx = 0; idx < output.size(); idx++) {
        output[idx] = static_cast<T>(input[idx]);
    }
    return output;
}
} // namespace MyBLAS

#endif // NE591_008_MATRIX_H
