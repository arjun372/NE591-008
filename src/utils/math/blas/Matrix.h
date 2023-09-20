/**
 * @file Matrix.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains my BLAS::Matrix implementation.
*/

#pragma once

#include <vector>
#include <iostream>

namespace MyBLAS {
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
        static Matrix Eye(size_t size) {
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
         * @brief Overloaded operator<< to print the matrix to an output stream.
         * @param os Output stream to print the matrix to.
         * @param m Matrix to print.
         * @return Reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
            for(size_t i = 0; i < m.getRows(); ++i) {
                for(size_t j = 0; j < m.getCols(); ++j) {
                    os << m[i][j] << ' ';
                }
                os << '\n';
            }
            return os;
        }
    };
}
