/**
 * @file LinearSolver.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the definition of the LinearSolver class and its associated methods.
 */

#ifndef NE591_008_LINEARSOLVER_H
#define NE591_008_LINEARSOLVER_H

#include "math/factorization/Factorize.h"
#include "math/relaxation/RelaxationMethods.h"
#include "utils/math/Stats.h"

#include <variant>

/**
 * @brief Namespace containing the definition of the LinearSolver class and its associated methods.
 */
namespace MyBLAS::Solver {

/**
 * @brief A variant type that can hold either a factorization method or a relaxation method.
 */
using Type = std::variant<MyFactorizationMethod::Type, MyRelaxationMethod::Type>;

/**
 * @brief A helper template that always evaluates to false.
 */
template <class...> inline constexpr bool always_false_v = false;

/**
 * @brief Function that returns the key of the type of the given value.
 * @param value The value whose type key is to be returned.
 * @return The key of the type of the given value.
 */
const char *TypeKey(const Type &value) {
    return std::visit(
        [](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, MyFactorizationMethod::Type>)
                return MyFactorizationMethod::TypeKey(arg);
            else if constexpr (std::is_same_v<T, MyRelaxationMethod::Type>)
                return MyRelaxationMethod::TypeKey(arg);
            else
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
        },
        value);
}

/**
 * @brief Structure that represents the solution of the linear solver.
 * @tparam T The type of the elements in the matrix/vector.
 */
template <typename T> struct Solution {

    explicit Solution<T>(MyBLAS::Vector<T> &initial) { x = initial; }

    explicit Solution<T>(size_t size) { x = MyBLAS::Vector<T>(size, 0); }

    Solution() = default;
    MyBLAS::Solver::Type method;
    bool converged = false;
    size_t iterations = std::numeric_limits<size_t>::quiet_NaN();
    T iterative_error = std::numeric_limits<T>::quiet_NaN();

    MyBLAS::Vector<T> x{};

    [[nodiscard]] MyBLAS::Vector<T> getResidual(MyBLAS::Matrix<T> a) const { return a * x; }

    [[nodiscard]] T getMaxResidual(MyBLAS::Matrix<T> a, MyBLAS::Vector<T> b) const {
        return MyBLAS::Stats::max<T>(MyBLAS::Stats::abs(b - getResidual(a)));
    }
};

} // namespace MyBLAS::Solver

#endif // NE591_008_LINEARSOLVER_H
