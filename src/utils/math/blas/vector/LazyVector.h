/**
* @file LazyVector.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
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
    using Generator = std::function<DataType(size_t)>;

    explicit LazyVector(size_t size, Generator generator)
        : size_(size), generator_(std::move(generator)) {}

    [[nodiscard]] size_t size() const { return size_; }

    DataType operator[](size_t index) const {
        return generator_(index);
    }

    // copy constructor
    LazyVector(const LazyVector& other) : size_(other.size_), generator_(other.generator_) {}

    // Assignment operator
    LazyVector& operator=(const LazyVector& other) {
        if (this != &other) {
            size_ = other.size_;
            generator_ = other.generator_;
        }
        return *this;
    }

    // Define the addition operation.
    friend LazyVector operator+(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x + y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    // Define the subtraction operation.
    friend LazyVector operator-(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x - y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    // Define the multiplication operation.
    friend LazyVector operator*(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x * y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    // Define the division operation.
    friend LazyVector operator/(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x / y; };
        VectorExpression<DataType, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector(a.size(), [expr](size_t i) { return expr[i]; });
    }

    // Define the scalar addition operation.
    LazyVector operator+(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] + scalar; });
    }

    // Define the scalar subtraction operation.
    LazyVector operator-(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] - scalar; });
    }

    // Define the scalar multiplication operation.
    LazyVector operator*(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] * scalar; });
    }

    // Define the scalar division operation.
    LazyVector operator/(DataType scalar) const {
        return LazyVector(size_, [this, scalar](size_t i) { return (*this)[i] / scalar; });
    }

    // Define the unary negation operation.
    LazyVector operator-() const {
        return LazyVector(size_, [this](size_t i) { return -(*this)[i]; });
    }

    // Define the in-place addition operation.
    LazyVector& operator+=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) + b[i]; };
        return *this;
    }

    // Define the in-place subtraction operation.
    LazyVector& operator-=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) - b[i]; };
        return *this;
    }

    // Define the in-place multiplication operation.
    LazyVector& operator*=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) * b[i]; };
        return *this;
    }

    // Define the in-place division operation.
    LazyVector& operator/=(LazyVector const& b) {
        auto old_gen = generator_;
        generator_ = [old_gen, b](size_t i) { return old_gen(i) / b[i]; };
        return *this;
    }

    // Define the in-place scalar addition operation.
    LazyVector& operator+=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) + scalar; };
        return *this;
    }

    // Define the in-place scalar subtraction operation.
    LazyVector& operator-=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) - scalar; };
        return *this;
    }

    // Define the in-place scalar multiplication operation.
    LazyVector& operator*=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) * scalar; };
        return *this;
    }

    // Define the in-place scalar division operation.
    LazyVector& operator/=(DataType scalar) {
        auto old_gen = generator_;
        generator_ = [this, old_gen, scalar](size_t i) { return old_gen(i) / scalar; };
        return *this;
    }

    // Define the element-wise equality comparison operation.
    friend LazyVector<bool> equal(LazyVector const& a, LazyVector const& b) {
        auto binary_op = [](DataType const& x, DataType const& y) { return x == y; };
        VectorExpression<bool, LazyVector, LazyVector, decltype(binary_op)> expr(a, b, binary_op);
        return LazyVector<bool>(a.size(), [expr](size_t i) { return expr[i]; });
    }

    // Define the equality comparison operation.
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

    // Define the inequality comparison operation.
    friend bool operator!=(LazyVector const& a, LazyVector const& b) {
        return !(a == b);
    }

  private:
    size_t size_;
    Generator generator_;
};
}

#endif // NE591_008_LAZYVECTOR_H
