/**
* @file LazyVector.h
* @brief Header file for the LazyVector class.
* @author Arjun Earthperson
* @date 10/13/2023
*
* This file contains the declaration of the LazyVector class, a vector that computes its values lazily.
 */

#ifndef NE591_008_LAZYVECTOR_H
#define NE591_008_LAZYVECTOR_H

#include <algorithm>
#include <cstddef>
#include <functional>

#include "VectorExpression.h"

namespace MyBLAS {

/**
 * @class LazyVector
 * @brief A vector that lazily computes its values.
 *
 * This class represents a vector whose values are computed on-the-fly each time they're accessed.
 * The computation is defined by a generator function provided at construction time.
 *
 * Note: This implementation does not cache computed values. Even though we're using the lazy evaluation
 * pattern here, we've made the choice to ignore caching of computed values for simplicity and to save memory.
 * This could be less efficient if the computation is expensive and the same values are accessed multiple times.
 * The choice between this approach and caching computed values depends on the specific requirements of your
 * application.
 */
template <typename DataType>
class LazyVector {
  public:
    /**
     * @brief A generator function type.
     * @param index The index for which to generate a value.
     * @return The computed value at the given index.
     */
    using Generator = std::function<DataType(size_t)>;

    /**
     * @brief Constructs a LazyVector with a given size and generator function.
     * @param size The size of the vector.
     * @param generator The generator function for computing values.
     */
    explicit LazyVector(size_t size, Generator generator)
        : size_(size), generator_(std::move(generator)) {}

    /**
     * @brief Get the size of the LazyVector.
     * @return The size of the vector.
     */
    [[nodiscard]] size_t size() const { return size_; }

    /**
     * @brief Access the element at a specific index.
     * @param index The index of the element to access.
     * @return The computed value at the given index.
     */
    DataType operator[](size_t index) const {
        return generator_(index);
    }

    /**
     * @brief Copy constructor for LazyVector.
     * @param other The LazyVector to copy from.
     */
    LazyVector(const LazyVector& other) : size_(other.size_), generator_(other.generator_) {}

    /**
     * @brief Assignment operator for LazyVector.
     * @param other The LazyVector to assign from.
     * @return A reference to this LazyVector after assignment.
     */
    LazyVector& operator=(const LazyVector& other) {
        if (this != &other) {
            size_ = other.size_;
            generator_ = other.generator_;
        }
        return *this;
    }

    // Binary operations

    /**
     * @brief Define the addition operation for LazyVector.
     * @param a The first LazyVector.
     * @param b The second LazyVector.
     * @return The LazyVector resulting from the addition of 'a' and 'b'.
     */
    friend LazyVector operator+(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x + y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    /**
     * @brief Define the subtraction operation for LazyVector.
     * @param a The first LazyVector.
     * @param b The second LazyVector.
     * @return The LazyVector resulting from the subtraction of 'a' and 'b'.
     */
    friend LazyVector operator-(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x - y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    /**
     * @brief Define the multiplication operation for LazyVector.
     * @param a The first LazyVector.
     * @param b The second LazyVector.
     * @return The LazyVector resulting from the multiplication of 'a' and 'b'.
     */
    friend LazyVector operator*(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x * y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    /**
     * @brief Define the division operation for LazyVector.
     * @param a The first LazyVector.
     * @param b The second LazyVector.
     * @return The LazyVector resulting from the division of 'a' and 'b'.
     */
    friend LazyVector operator/(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x / y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    // Scalar operations

    /**
     * @brief Define the scalar addition operation for LazyVector.
     * @param scalar The scalar value to add.
     * @return The LazyVector resulting from adding 'scalar' to this LazyVector.
     */
    LazyVector operator+(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] + scalar; });
    }

    /**
     * @brief Define the scalar subtraction operation for LazyVector.
     * @param scalar The scalar value to subtract.
     * @return The LazyVector resulting from subtracting 'scalar' from this LazyVector.
     */
    LazyVector operator-(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] - scalar; });
    }

    /**
     * @brief Define the scalar multiplication operation for LazyVector.
     * @param scalar The scalar value to multiply by.
     * @return The LazyVector resulting from multiplying this LazyVector by 'scalar'.
     */
    LazyVector operator*(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] * scalar; });
    }

    /**
     * @brief Define the scalar division operation for LazyVector.
     * @param scalar The scalar value to divide by.
     * @return The LazyVector resulting from dividing this LazyVector by 'scalar'.
     */
    LazyVector operator/(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] / scalar; });
    }

    /**
     * @brief Define the unary negation operation for LazyVector.
     * @return The LazyVector resulting from negating this LazyVector.
     */
    LazyVector operator-() const {
        return LazyVector(size_, [this](size_t i) { return -(*this)[i]; });
    }

    // In-place operations

    /**
     * @brief Define the in-place addition operation for LazyVector.
     * @param b The LazyVector to add to this LazyVector.
     * @return A reference to this LazyVector after the addition operation.
     */
    LazyVector& operator+=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) + b[i]; };
        return *this;
    }

    /**
     * @brief Define the in-place subtraction operation for LazyVector.
     * @param b The LazyVector to subtract from this LazyVector.
     * @return A reference to this LazyVector after the subtraction operation.
     */
    LazyVector& operator-=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) - b[i]; };
        return *this;
    }

    /**
     * @brief Define the in-place multiplication operation for LazyVector.
     * @param b The LazyVector to multiply with this LazyVector.
     * @return A reference to this LazyVector after the multiplication operation.
     */
    LazyVector& operator*=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) * b[i]; };
        return *this;
    }

    /**
     * @brief Define the in-place division operation for LazyVector.
     * @param b The LazyVector to divide this LazyVector by.
     * @return A reference to this LazyVector after the division operation.
     */
    LazyVector& operator/=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) / b[i]; };
        return *this;
    }

    /**
     * @brief Define the in-place scalar addition operation for LazyVector.
     * @param scalar The scalar value to add to this LazyVector.
     * @return A reference to this LazyVector after the addition operation.
     */
    LazyVector& operator+=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) + scalar; };
        return *this;
    }

    /**
     * @brief Define the in-place scalar subtraction operation for LazyVector.
     * @param scalar The scalar value to subtract from this LazyVector.
     * @return A reference to this LazyVector after the subtraction operation.
     */
    LazyVector& operator-=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) - scalar; };
        return *this;
    }

    /**
     * @brief Define the in-place scalar multiplication operation for LazyVector.
     * @param scalar The scalar value to multiply this LazyVector by.
     * @return A reference to this LazyVector after the multiplication operation.
     */
    LazyVector& operator*=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) * scalar; };
        return *this;
    }

    /**
     * @brief Define the in-place scalar division operation for LazyVector.
     * @param scalar The scalar value to divide this LazyVector by.
     * @return A reference to this LazyVector after the division operation.
     */
    LazyVector& operator/=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) / scalar; };
        return *this;
    }

    // Comparison operations

    /**
     * @brief Define the element-wise equality comparison operation for LazyVector.
     * @param a The first LazyVector.
     * @param b The second LazyVector.
     * @return A LazyVector of bools indicating element-wise equality.
     */
    friend LazyVector<bool> equal(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x == y; };
        VectorExpression<bool, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector<bool>(a.size(), [expr](size_t i) { return expr[i]; });
    }

    /**
     * @brief Define the equality comparison operation for LazyVector.
     * @param a The first LazyVector.
     * @param b The second LazyVector.
     * @return true if the LazyVectors are equal, false otherwise.
     */
    friend bool operator==(LazyVector const& a, LazyVector const& b) {
        if (a.size() != b.size()) {
            return false;
        }
        auto eq = equal(a, b);
        for (size_t i = 0; i < eq.size(); ++i) {
            if (!eq[i]) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Define the inequality comparison operation for LazyVector.
     * @param a The first LazyVector.
     * @param b The second LazyVector.
     * @return true if the LazyVectors are not equal, false if they are equal.
     */
    friend bool operator!=(LazyVector const& a, LazyVector const& b) {
        return !(a == b);
    }

  private:
    size_t size_; ///< The size of the LazyVector.
    Generator generator_; ///< The generator function for computing values.
};

}

#endif // NE591_008_LAZYVECTOR_H
