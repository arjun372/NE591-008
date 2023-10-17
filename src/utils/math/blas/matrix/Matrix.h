/**
 * @file Matrix.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains my BLAS::Matrix declarations.
 */

#ifndef NE591_008_MATRIX_H
#define NE591_008_MATRIX_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "math/blas/vector/Vector.h"
#include "math/blas/Constants.h"

namespace MyBLAS {

template <typename T> class Matrix;
template <typename T> class Vector;

/**
 * @class Matrix
 * @brief Class representing a matrix of T type values.
 */
template <typename T = MyBLAS::NumericType> class Matrix {

  protected:
    std::vector<std::vector<T>> data; ///< 2D vector representing the matrix data.

  public:
    /**
     * @brief Default constructor. Initializes an empty matrix.
     */
    Matrix() = default;

    /**
     * @brief Default virtual destructor.
     */
    virtual ~Matrix() = default;

    /**
     * @brief Constructor that initializes the matrix with a given 2D vector.
     * @param _data 2D vector to initialize the matrix with.
     */
    explicit Matrix(std::vector<std::vector<T>> &_data) : data(_data) {}

    /**
     * @brief Constructor that initializes the matrix with a given 2D vector.
     * @param _data 2D vector to initialize the matrix with.
     */
    explicit Matrix(std::vector<std::vector<T>> _data) : data(_data) {}

    /**
     * @brief Parameterized constructor that initializes the matrix with a given size and initial value.
     * @param _rows Number of rows in the matrix.
     * @param _cols Number of columns in the matrix.
     * @param _initial Initial value for all elements in the matrix.
     */
    Matrix(size_t _rows, size_t _cols, const T _initial = 0)
        : data(_rows, std::vector<T>(_cols, _initial)) {}

    /**
     * @brief Constructor that initializes the matrix with a given initializer list.
     * @param initList Initializer list to initialize the matrix with.
     */
    Matrix(std::initializer_list<std::initializer_list<T>> initList) {
        data.resize(initList.size());
        size_t i = 0;
        for (const auto &row : initList) {
            data[i].resize(initList.begin()->size());
            size_t j = 0;
            for (const auto &elem : row) {
                data[i][j] = elem;
                ++j;
            }
            ++i;
        }
    }

    virtual /**
     * @brief Overloaded operator[] to access individual rows of the matrix.
     * @param rowNum Index of the row to access.
     * @return Reference to the row at the given index.
     */
    std::vector<T> &operator[](const size_t rowNum) {
        assert(rowNum < getRows());
        return data[rowNum];
    }

    virtual /**
     * @brief Overloaded operator[] to access individual rows of the matrix (const version).
     * @param rowNum Index of the row to access.
     * @return Const reference to the row at the given index.
     */
    const std::vector<T> &operator[](const size_t rowNum) const {
        assert(rowNum < getRows());
        return data[rowNum];
    }

    /**
     * @brief Getter for the matrix data.
     * @return Const reference to the matrix data.
     */
    [[nodiscard]] const std::vector<std::vector<T>> &getData() const { return data; }

    /**
     * @brief Getter for the vector data.
     * @return reference to the vector data.
     */
    [[nodiscard]] std::vector<std::vector<T>> &getData() { return data; }

    /**
     * @brief Adds a new row to the end of the matrix.
     * @param row Vector representing the new row to be added.
     */
    void push_back(const std::vector<T> &row) {
        data.push_back(row);
    }

    [[nodiscard]] virtual /**
     * @brief Getter for the number of rows in the matrix.
     * @return Number of rows in the matrix.
     */
    inline size_t getRows() const {
        return data.size();
    }

    [[nodiscard]] virtual /**
     * @brief Getter for the number of columns in the matrix.
     * @return Number of columns in the matrix.
     */
    inline size_t getCols() const {
        return data.empty() ? 0 : data[0].size();
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
        assert(getRows() == rhs.getRows());
        assert(getCols() == rhs.getCols());
        const size_t rows = getRows(), cols = getCols();
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
        assert(getRows() == rhs.getRows());
        assert(getCols() == rhs.getCols());
        const size_t rows = getRows(), cols = getCols();
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
        assert(getCols() == rhs.getRows());
        const size_t my_rows = getRows(), my_cols = getCols();
        const size_t rhs_cols = rhs.getCols();
        Matrix result(my_rows, rhs_cols, 0);
        for (size_t i = 0; i < my_rows; ++i) {
            for (size_t j = 0; j < rhs_cols; ++j) {
                for (size_t k = 0; k < my_cols; ++k) {
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
        assert(getCols() == rhs.size());
        const size_t my_rows = getRows(), my_cols = getCols();
        Vector<T> result(my_rows, 0);
        for (size_t i = 0; i < my_rows; ++i) {
            for (size_t j = 0; j < my_cols; ++j) {
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
        const size_t subRows = subMatrix.getRows(), subCols = subMatrix.getCols();
        assert(rowStart + subRows <= getRows());
        assert(colStart + subCols <= getCols());
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
        assert(rowStart + subRows <= getRows());
        assert(colStart + subCols <= getCols());
        Matrix<T> subMatrix(subRows, subCols, 0);
        for (size_t i = 0; i < subRows; ++i) {
            for (size_t j = 0; j < subCols; ++j) {
                subMatrix[i][j] = data[rowStart + i][colStart + j];
            }
        }

        return subMatrix;
    }

    /**
     * @brief Overloaded operator== to check for equality between two matrices.
     * @param rhs Matrix to compare with the current matrix.
     * @return True if the matrices are equal, false otherwise.
     *
     * @note This operator checks for exact equality of each element in the matrices.
     *       However, when dealing with floating-point numbers, due to the way they are
     *       represented in computers, two numbers that are theoretically equal may not
     *       be exactly equal when represented in a computer. Therefore, when dealing with
     *       floating-point numbers, it might be more appropriate to check if the difference
     *       between the matrices is below a certain tolerance, rather than checking for exact equality.
     */
    bool operator==(const Matrix &rhs) const {
        const size_t my_rows = getRows(), my_cols = getCols(), rhs_rows = rhs.getRows(), rhs_cols = rhs.getCols();
        if (my_rows != rhs_rows || my_cols != rhs_cols) {
            return false;
        }
        for (size_t i = 0; i < my_rows; ++i) {
            for (size_t j = 0; j < my_cols; ++j) {
                if (this->data[i][j] != rhs.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * @brief Overloaded operator* to multiply the matrix by a scalar.
     * @param scalar Scalar to multiply the matrix with.
     * @return Resultant matrix after multiplication.
     */
    Matrix operator*(const T &scalar) const {
        const size_t rows = getRows(), cols = getCols();
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
        assert(scalar != 0);
        const size_t rows = getRows(), cols = getCols();
        Matrix result(rows, cols, 0);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = this->data[i][j] / scalar;
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
        const size_t rows = getRows(), cols = getCols();
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
        const size_t rows = getRows(), cols = getCols();
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
        assert(row1 < getRows() && row2 < getRows());
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
        const size_t rows = m.getRows(), cols = m.getCols();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
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
template <template<typename> class M, template<typename> class V, typename T> V<T> forwardSubstitution(const M<T> &L, const V<T> &b) {
    static_assert(std::is_same<M<T>, MyBLAS::Matrix<T>>::value, "M must be a MyBLAS::Matrix type");
    static_assert(std::is_same<V<T>, MyBLAS::Vector<T>>::value, "M must be a MyBLAS::Matrix type");
    const auto n = b.size();
    auto y = V<T>(n);
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
template <template<typename> class M, template<typename> class V, typename T> V<T> backwardSubstitution(const M<T> &U, const V<T> &y) {
    static_assert(std::is_same<M<T>, MyBLAS::Matrix<T>>::value, "M must be a MyBLAS::Matrix type");
    static_assert(std::is_same<V<T>, MyBLAS::Vector<T>>::value, "M must be a MyBLAS::Matrix type");
    const auto n = y.size();
    auto x = V<T>(n, static_cast<T>(0));
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
