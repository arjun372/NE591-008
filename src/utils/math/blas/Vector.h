/**
 * @file Vector.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains my BLAS::Vector implementation.
*/

#ifndef NE591_008_VECTOR_H
#define NE591_008_VECTOR_H

#include <vector>
#include <iostream>
#include <iomanip>

namespace MyBLAS {
    /**
 * @class Vector
 * @brief Class representing a vector of T values.
 */
    template <typename T>
    class Vector {

    protected:
        std::vector<T> data; ///< Vector representing the vector data.
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
        explicit Vector(std::vector<T> &_data, bool _isRow) : data(_data), isRow(_isRow) {}

        /**
         * @brief Constructor that initializes the vector with a given vector and row/column indicator.
         * @param _data Vector to initialize the vector with.
         * @param _isRow Boolean indicating whether the vector is a row vector.
         */
        explicit Vector(std::vector<T> _data, bool _isRow) : data(std::move(_data)), isRow(_isRow) {}

        /**
         * @brief Constructor that initializes the vector with a given vector. The vector is assumed to be a column vector.
         * @param _data Vector to initialize the vector with.
         */
        explicit Vector(std::vector<T> &_data) : data(_data), isRow(false) {}

        /**
         * @brief Parameterized constructor that initializes the vector with a given size, initial value, and row/column indicator.
         * @param size Size of the vector.
         * @param initial Initial value for all elements in the vector.
         * @param _isRow Boolean indicating whether the vector is a row vector.
         */
        explicit Vector(size_t size, const T initial = 0, bool _isRow = false) : data(size, initial), isRow(_isRow) {}

        /**
         * @brief Getter for the vector data.
         * @return Const reference to the vector data.
         */
        [[nodiscard]] const std::vector<T> &getData() const {
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
        T& operator[](const size_t idx) {
            return data[idx];
        }

        /**
         * @brief Overloaded operator[] to access individual elements of the vector (const version).
         * @param idx Index of the element to access.
         * @return Const reference to the element at the given index.
         */
        const T& operator[](const size_t idx) const {
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
        T operator*(const Vector& rhs) const {
            if (data.size() != rhs.size()) {
                throw std::exception();
            }
            T result = 0;
            for (size_t i = 0; i < size(); ++i) {
                result += this->data[i] * rhs.data[i];
            }
            return result;
        }

        /**
         * @brief Overloaded operator* to multiply a vector with a scalar.
         * @param scalar Scalar to multiply with the vector.
         * @return Resultant vector after multiplication.
         */
        Vector<T> operator*(const T& scalar) const {
            Vector<T> result(size(), 0);
            for (size_t i = 0; i < size(); ++i) {
                result[i] = data[i] * scalar;
            }
            return result;
        }

        /**
         * @brief Overloaded operator/ to divide a vector by a scalar.
         * @param scalar Scalar to divide the vector by.
         * @return Resultant vector after division.
         */
        Vector<T> operator/(const T& scalar) const {
            Vector<T> result(size(), 0);
            for (size_t i = 0; i < size(); ++i) {
                result[i] = data[i] / scalar;
            }
            return result;
        }

        /**
         * @brief Overloaded operator+ to add a scalar to a vector.
         * @param scalar Scalar to add to the vector.
         * @return Resultant vector after addition.
         */
        Vector<T> operator+(const T& scalar) const {
            Vector<T> result(size(), 0);
            for (size_t i = 0; i < size(); ++i) {
                result[i] = data[i] + scalar;
            }
            return result;
        }

        /**
         * @brief Overloaded operator- to subtract a scalar from a vector.
         * @param scalar Scalar to subtract from the vector.
         * @return Resultant vector after subtraction.
         */
        Vector<T> operator-(const T& scalar) const {
            Vector<T> result(size(), 0);
            for (size_t i = 0; i < size(); ++i) {
                result[i] = data[i] - scalar;
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
                os << std::setw(width) << std::setfill(' ') << std::scientific << static_cast<long double>(m[i]);
            }
            os << '\n';
            return os;
        }
    };

    /**
     * @brief Calculates the minimum value of the vector.
     * @return Minimum value of the vector elements.
     */
    template <typename T>
    static T min(const Vector<T>& a) {
        if(a.getData().empty()) {
            return NAN;
        }
        T minValue = a[0];
        for (size_t i = 1; i < a.size(); ++i) {
            minValue = std::min(minValue, a[i]);
        }
        return minValue;
    }

    /**
     * @brief Calculates the maximum value of the vector.
     * @return Minimum value of the vector elements.
     */
    template <typename T>
    static T max(const Vector<T>& a) {
        if(a.getData().empty()) {
            return NAN;
        }
        T maxValue = a[0];
        for (size_t i = 1; i < a.size(); ++i) {
            maxValue = std::max(maxValue, a[i]);
        }
        return maxValue;
    }
}

#endif //NE591_008_VECTOR_H
