/**
 * @file LinearSolver.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief TODO:: DOCUMENT
*/

#ifndef NE591_008_LINEARSOLVER_H
#define NE591_008_LINEARSOLVER_H

#include "math/factorization/Factorize.h"
#include "math/relaxation/RelaxationMethods.h"

#include <variant>

// TODO:: DOCUMENT
namespace MyLinearSolvingMethod {

    // TODO:: DOCUMENT
    using Type = std::variant<MyFactorizationMethod::Type, MyRelaxationMethod::Type>;

    // TODO:: DOCUMENT
    template<class...> inline constexpr bool always_false_v = false;

    // TODO :: DOCUMENT
    const char *TypeKey(const Type& value) {
        return std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, MyFactorizationMethod::Type>)
                return MyFactorizationMethod::TypeKey(arg);
            else if constexpr (std::is_same_v<T, MyRelaxationMethod::Type>)
                return MyRelaxationMethod::TypeKey(arg);
            else
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }, value);
    }

    // TODO:: DOCUMENT
    template <typename T>
    struct Input {
        size_t n = std::numeric_limits<size_t>::quiet_NaN(); ////< Size of the matrix/vector
        T threshold = std::numeric_limits<T>::quiet_NaN(); ////< The convergence threshold
        T relaxation_factor = std::numeric_limits<T>::quiet_NaN();; ////< The relaxation factor, also known as the optimal weight or omega, ω
        size_t max_iterations = std::numeric_limits<size_t>::quiet_NaN();; ////< Maximum number of iterations to perform
        MyBLAS::Matrix<T> coefficients{}; ///< Coefficient matrix A
        MyBLAS::Vector<T> constants{}; ///< Vector of constants b.
    };


    // TODO:: DOCUMENT
    template <typename T>
    struct Solution {

        explicit Solution<T>(MyBLAS::Vector<T> &initial) {
            x = initial;
        }

        explicit Solution<T>(size_t size) {
            x = MyBLAS::Vector<T>(size, 0);
        }

        Solution() = default;
        MyLinearSolvingMethod::Type method;
        bool converged = false;
        size_t iterations = std::numeric_limits<size_t>::quiet_NaN();
        T iterative_error = std::numeric_limits<T>::quiet_NaN();;
        MyBLAS::Vector<T> x{};


        [[nodiscard]] MyBLAS::Vector<T> getResidual(MyBLAS::Matrix<T> a) const {
            return a * x;
        }

        [[nodiscard]] T getMaxResidual(MyBLAS::Matrix<T> a, MyBLAS::Vector<T> b) const {
            return MyBLAS::max<T>(MyBLAS::abs(b - getResidual(a)));
        }

    };

}

#endif //NE591_008_LINEARSOLVER_H
