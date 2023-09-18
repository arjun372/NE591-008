/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief This file contains the input and output definitions for this project.
 */

#pragma once

#include <utility>

namespace myBLAS {


    class Matrix {

    protected:
        std::vector<std::vector<long double>> data;
        size_t rows, cols;

    public:

        // Default constructor
        Matrix() : rows(0), cols(0) {}

        explicit Matrix(std::vector<std::vector<long double>> &_data) : data(_data), rows(_data.size()), cols(_data.size()) {}

        explicit Matrix(std::vector<std::vector<long double>> _data) : data(_data), rows(_data.size()), cols(_data.size()) {}

        // Parameterized constructor
        Matrix(size_t _rows, size_t _cols, const long double _initial) : data(_rows, std::vector<long double>(_cols, _initial)), rows(_rows), cols(_cols) {}

        // Access the individual elements
        std::vector<long double>& operator[](const size_t rowNum) {
            return data[rowNum];
        }

        // Access the individual elements (const)
        const std::vector<long double>& operator[](const size_t rowNum) const {
            return data[rowNum];
        }

        [[nodiscard]] const std::vector<std::vector<long double>> &getData() const {
            return data;
        }

        // Get the number of row and columns
        [[nodiscard]] size_t getRows() const { return rows; }
        [[nodiscard]] size_t getCols() const { return cols; }

        // Get the identity matrix
        static Matrix Eye(size_t size) {
            Matrix eye(size, size, 0);
            for(size_t i = 0; i < size; ++i) {
                eye[i][i] = 1;
            }
            return eye;
        }

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


    class Vector {

    protected:
        std::vector<long double> data;
        bool isRow;

    public:

        // Default constructor
        Vector() : isRow(false) {}

        explicit Vector(std::vector<long double> &_data, bool _isRow) : data(_data), isRow(_isRow) {}

        explicit Vector(std::vector<long double> _data, bool _isRow) : data(std::move(_data)), isRow(_isRow) {}

        explicit Vector(std::vector<long double> &_data) : data(_data), isRow(false) {}

        // Parameterized constructor
        explicit Vector(size_t size, const long double initial = 0, bool _isRow = false) : data(size, initial), isRow(_isRow) {}

        [[nodiscard]] const std::vector<long double> &getData() const {
            return data;
        }

        [[nodiscard]] size_t size() const {
            return data.size();
        }

        long double& operator[](const size_t idx) {
            return data[idx];
        }

        // Access the individual elements (const)
        const long double& operator[](const size_t idx) const {
            return data[idx];
        }

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

        friend std::ostream& operator<<(std::ostream& os, const Vector& m) {
            for(size_t i = 0; i < m.size(); ++i) {
                os << m[i] << " ";
            }
            os << '\n';
            return os;
        }
    };

    // TODO: Document
    typedef struct Input {
        Input() {}

        size_t n = 0;
        Matrix LU;
        Matrix lower;
        Matrix upper;
        Vector constants;

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


    // TODO:: Document
    typedef struct Output {
        Output() = default;
        Vector x;
        void toJSON(nlohmann::json &jsonMap) const {
            jsonMap["x"] = x.getData();
        }
    } OutputVector;

}


typedef std::map<std::string, std::string> Dictionary;
