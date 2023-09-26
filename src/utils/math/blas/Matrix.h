/**
 * @file Matrix.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains my BLAS::Matrix declarations.
*/

#ifndef NE591_008_MATRIX_H
#define NE591_008_MATRIX_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "Vector.h"

namespace MyBLAS {

    class Matrix;
    class Vector;

    /**
     * @class Matrix
     * @brief Class representing a matrix of long double values.
     */
    class Matrix {

    protected:
        std::vector<std::vector<long double>> data; ///< 2D vector representing the matrix data.
        size_t rows, cols; ///< Number of rows and columns in the matrix.

    public:

        /**
         * @brief Default constructor. Initializes an empty matrix.
         */
        Matrix() : rows(0), cols(0) {}

        /**
         * @brief Constructor that initializes the matrix with a given 2D vector.
         * @param _data 2D vector to initialize the matrix with.
         */
        explicit Matrix(std::vector<std::vector<long double>> &_data) : data(_data), rows(_data.size()), cols(_data.size()) {}

        /**
         * @brief Constructor that initializes the matrix with a given 2D vector.
         * @param _data 2D vector to initialize the matrix with.
         */
        explicit Matrix(std::vector<std::vector<long double>> _data) : data(_data), rows(_data.size()), cols(_data.size()) {}

        /**
         * @brief Parameterized constructor that initializes the matrix with a given size and initial value.
         * @param _rows Number of rows in the matrix.
         * @param _cols Number of columns in the matrix.
         * @param _initial Initial value for all elements in the matrix.
         */
        Matrix(size_t _rows, size_t _cols, const long double _initial) : data(_rows, std::vector<long double>(_cols, _initial)), rows(_rows), cols(_cols) {}

        /**
         * @brief Overloaded operator[] to access individual rows of the matrix.
         * @param rowNum Index of the row to access.
         * @return Reference to the row at the given index.
         */
        std::vector<long double>& operator[](const size_t rowNum) {
            return data[rowNum];
        }

        /**
         * @brief Overloaded operator[] to access individual rows of the matrix (const version).
         * @param rowNum Index of the row to access.
         * @return Const reference to the row at the given index.
         */
        const std::vector<long double>& operator[](const size_t rowNum) const {
            return data[rowNum];
        }

        /**
         * @brief Getter for the matrix data.
         * @return Const reference to the matrix data.
         */
        [[nodiscard]] const std::vector<std::vector<long double>> &getData() const {
            return data;
        }

        /**
         * @brief Getter for the number of rows in the matrix.
         * @return Number of rows in the matrix.
         */
        [[nodiscard]] size_t getRows() const { return rows; }

        /**
         * @brief Getter for the number of columns in the matrix.
         * @return Number of columns in the matrix.
         */
        [[nodiscard]] size_t getCols() const { return cols; }

        /**
         * @brief Static function to get the identity matrix of a given size.
         * @param size Size of the identity matrix.
         * @return Identity matrix of the given size.
         */
        static Matrix eye(size_t size) {
            Matrix eye(size, size, 0);
            for(size_t i = 0; i < size; ++i) {
                eye[i][i] = 1;
            }
            return eye;
        }

        /**
         * @brief Overloaded operator+ to add two matrices.
         * @param rhs Matrix to add to the current matrix.
         * @return Resultant matrix after addition.
         */
        Matrix operator+(const Matrix& rhs) const {
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
        Matrix operator-(const Matrix& rhs) const {
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
        Matrix operator*(const Matrix& rhs) const {
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
        Vector operator*(const Vector& rhs) const {
            if (cols != rhs.size()) {
                throw std::exception();
            }
            Vector result(rows, 0);
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < cols; ++j) {
                    result[i] += this->data[i][j] * rhs[j];
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
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
            const auto width = static_cast<int>(std::cout.precision() + static_cast<std::streamsize>(10));
            for(size_t i = 0; i < m.getRows(); ++i) {
                for(size_t j = 0; j < m.getCols(); ++j) {
                    os << std::setw(width) << std::setfill(' ') << std::scientific << m[i][j];
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
    template<typename T>
    static MyBLAS::Vector forwardSubstitution(const MyBLAS::Matrix &L, const MyBLAS::Vector &b) {
        const auto n = b.size();
        MyBLAS::Vector y(n);
        for (size_t row = 0; row < n; row++) {
            T sum = 0.0f;
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
    template<typename T>
    static MyBLAS::Vector backwardSubstitution(const MyBLAS::Matrix &U, const MyBLAS::Vector &y) {
        const auto n = static_cast<int64_t>(y.size());
        MyBLAS::Vector x(n);
        for (int64_t i = n - 1; i >= 0; i--) {
            T sum = 0.0f;
            for (int64_t j = i + 1; j < n; j++) {
                sum += U[i][j] * x[j];
            }
            x[i] = (y[i] - sum) / U[i][i];
        }
        return x;
    }
} // MyBLAS

#endif //NE591_008_MATRIX_H
