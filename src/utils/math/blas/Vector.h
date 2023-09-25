/**
 * @file Vector.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains my BLAS::Vector implementation.
*/

#pragma once

#include <vector>
#include <iostream>


namespace MyBLAS {
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
         * @brief Overloaded operator* to multiply two vectors (dot product).
         * @param rhs Vector to multiply with the current vector.
         * @return Resultant scalar after multiplication.
         */
        long double operator*(const Vector& rhs) const {
            if (data.size() != rhs.size()) {
                throw std::exception();
            }
            long double result = 0;
            for (size_t i = 0; i < size(); ++i) {
                result += this->data[i] * rhs.data[i];
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
            const auto width = static_cast<int>(std::cout.precision() + static_cast<std::streamsize>(10));
            for(size_t i = 0; i < m.size(); ++i) {
                os << std::setw(width) << std::setfill(' ') << std::scientific << m[i];
            }
            os << '\n';
            return os;
        }
    };

}
