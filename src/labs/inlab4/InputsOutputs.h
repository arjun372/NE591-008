/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief This file contains the input and output definitions for this project.
 */

#pragma once

#include <utility>

/**
 * @namespace myBLAS
 * @brief Namespace for the Basic Linear Algebra Subprograms (BLAS) library.
 */
namespace myBLAS {

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

    /**
     * @class Vector
     * @brief Class representing a vector of long double values.
     */
    class Vector {

    protected:
        std::vector<long double> data; ///< Vector representing the vector data.
        bool isRow; ///< Boolean indicating whether the vector is a row vector.

    public:

        /**
         * @brief Default constructor. Initializes an empty column vector.
         */
        Vector() : isRow(false) {}

        /**
         * @brief Constructor that initializes the vector with a given vector and row/column indicator.
         * @param _data Vector to initialize the vector with.
         * @param _isRow Boolean indicating whether the vector is a row vector.
         */
        explicit Vector(std::vector<long double> &_data, bool _isRow) : data(_data), isRow(_isRow) {}

        /**
         * @brief Constructor that initializes the vector with a given vector and row/column indicator.
         * @param _data Vector to initialize the vector with.
         * @param _isRow Boolean indicating whether the vector is a row vector.
         */
        explicit Vector(std::vector<long double> _data, bool _isRow) : data(std::move(_data)), isRow(_isRow) {}

        /**
         * @brief Constructor that initializes the vector with a given vector. The vector is assumed to be a column vector.
         * @param _data Vector to initialize the vector with.
         */
        explicit Vector(std::vector<long double> &_data) : data(_data), isRow(false) {}

        /**
         * @brief Parameterized constructor that initializes the vector with a given size, initial value, and row/column indicator.
         * @param size Size of the vector.
         * @param initial Initial value for all elements in the vector.
         * @param _isRow Boolean indicating whether the vector is a row vector.
         */
        explicit Vector(size_t size, const long double initial = 0, bool _isRow = false) : data(size, initial), isRow(_isRow) {}

        /**
         * @brief Getter for the vector data.
         * @return Const reference to the vector data.
         */
        [[nodiscard]] const std::vector<long double> &getData() const {
            return data;
        }

        /**
         * @brief Getter for the size of the vector.
         * @return Size of the vector.
         */
        [[nodiscard]] size_t size() const {
            return data.size();
        }

        /**
         * @brief Overloaded operator[] to access individual elements of the vector.
         * @param idx Index of the element to access.
         * @return Reference to the element at the given index.
         */
        long double& operator[](const size_t idx) {
            return data[idx];
        }

        /**
         * @brief Overloaded operator[] to access individual elements of the vector (const version).
         * @param idx Index of the element to access.
         * @return Const reference to the element at the given index.
         */
        const long double& operator[](const size_t idx) const {
            return data[idx];
        }

        /**
         * @brief Overloaded operator+ to add two vectors.
         * @param rhs Vector to add to the current vector.
         * @return Resultant vector after addition.
         */
        Vector operator+(const Vector& rhs) const {
            if (data.size() != rhs.size()) {
                throw std::exception();
            }
            Vector result(size(), 0);
            for (size_t i = 0; i < size(); ++i) {
                result[i] = this->data[i] + rhs.data[i];
            }
            return result;
        }

        /**
         * @brief Overloaded operator- to subtract two vectors.
         * @param rhs Vector to subtract from the current vector.
         * @return Resultant vector after subtraction.
         */
        Vector operator-(const Vector& rhs) const {
            if (data.size() != rhs.size()) {
                throw std::exception();
            }
            Vector result(size(), 0);
            for (size_t i = 0; i < size(); ++i) {
                result[i] = this->data[i] - rhs.data[i];
            }
            return result;
        }

        /**
         * @brief Overloaded operator<< to print the vector to an output stream.
         * @param os Output stream to print the vector to.
         * @param m Vector to print.
         * @return Reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Vector& m) {
            for(size_t i = 0; i < m.size(); ++i) {
                os << m[i] << " ";
            }
            os << '\n';
            return os;
        }
    };

    /**
     * @struct Input
     * @brief Struct representing the input matrices for the BLAS library.
     */
    typedef struct Input {
        Input() = default;

        size_t n = 0; ///< Size of the matrices.
        Matrix LU; ///< LU matrix.
        Matrix lower; ///< Lower triangular matrix.
        Matrix upper; ///< Upper triangular matrix.
        Vector constants; ///< Vector of constants.

        /**
        * @brief Converts the input parameters to a JSON object.
        * @param jsonMap A reference to the JSON object to store the input parameters.
        */
        void toJSON(nlohmann::json &jsonMap) const {
            jsonMap["n"] = n;
            jsonMap["lower"] = lower.getData();
            jsonMap["upper"] = upper.getData();
            jsonMap["LU"] = LU.getData();
            jsonMap["constants"] = constants.getData();
        }
    } InputMatrices;

    /**
     * @struct Output
     * @brief Struct representing the output vector for the BLAS library.
     */
    typedef struct Output {
        Output() = default;
        Vector x; ///< Output vector.

        /**
         * @brief Converts the output vector to a JSON object.
         * @param jsonMap A reference to the JSON object to store the output vector.
         */
        void toJSON(nlohmann::json &jsonMap) const {
            jsonMap["x"] = x.getData();
        }
    } OutputVector;

}
