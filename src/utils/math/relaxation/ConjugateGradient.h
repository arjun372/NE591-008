/**
 * @file ConjugateGradient.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the implementation of the CG method for solving systems of linear equations.
 */

#ifndef NE591_008_CONJUGATEGRADIENT_H
#define NE591_008_CONJUGATEGRADIENT_H

#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"
#include <cstddef>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <stdexcept>

#include "math/blas/Ops.h"

#include "math/LinearSolver.h"
#include "math/relaxation/RelaxationMethods.h"

/**
 * @namespace MyRelaxationMethod
 * @brief This namespace contains implementations of various relaxation methods for solving systems of linear equations.
 */
namespace MyRelaxationMethod {

#include <cmath>
#include <limits>
#include <stdexcept>

//template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
//MyLinearSolvingMethod::Solution<T> applyConjugateGradient(const MatrixType<T> &A, const VectorType<T> &b, const size_t max_iterations, const T tolerance) {
//
//    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
//    MyLinearSolvingMethod::Solution<T> results(n); // Initialize the results object with the size of the matrix
//
//    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
//    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum
//
//    VectorType<T> new_x(n, 0); // Initialize a new vector for the updated solution
//
//    VectorType<T> residual_vector = b - (A * results.x); // Initial residual
//    VectorType<T> search_direction = residual_vector;    // Initial search direction
//    T old_residual = residual_vector * residual_vector;  // Dot product of residual with itself
//    T new_residual = 0, alpha = 0;
//
//    for (results.iterations = 0; results.iterations < max_iterations; ++(results.iterations)) {
//        VectorType<T> Ap = A * search_direction;
//        alpha = old_residual / (search_direction * Ap);
//        results.x = results.x + (search_direction * alpha); // Update the solution vector
//        residual_vector = residual_vector - (Ap * alpha);   // Update the residual
//
//        new_residual = residual_vector * residual_vector;   // New dot product of residual with itself
//
//        iterative_error_squared = MyBLAS::L2(new_x, results.x, n);
//
//        if (new_residual < tolerance_squared) {
//            results.converged = true;
//            break; // Convergence achieved
//        }
//
//        p = r + (new_residual / old_residual) * p;       // Update the search direction
//        old_residual = new_residual;                     // Update the dot product for the next iteration
//    }
//
//    results.iterative_error = std::sqrt(new_residual); // Final error is the L2 norm of the residual
//    return results;
//}

// Function prototypes
//double dot_product(const std::vector<double>& v1, const std::vector<double>& v2);
//std::vector<double> matrix_vector_multiply(const std::vector<std::vector<double>>& A, const std::vector<double>& v);
//std::vector<double> vector_add(const std::vector<double>& v1, const std::vector<double>& v2, double alpha);
//std::vector<double> vector_subtract(const std::vector<double>& v1, const std::vector<double>& v2);
//std::vector<double> scalar_vector_multiply(const std::vector<double>& v, double alpha);

//// Helper functions
//double dot_product(const std::vector<double>& v1, const std::vector<double>& v2) {
//    return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
//}
//
//std::vector<double> matrix_vector_multiply(const std::vector<std::vector<double>>& A, const std::vector<double>& v) {
//    std::size_t n = A.size();
//    std::vector<double> result(n, 0.0);
//    for (std::size_t i = 0; i < n; ++i) {
//        for (std::size_t j = 0; j < n; ++j) {
//            result[i] += A[i][j] * v[j];
//        }
//    }
//    return result;
//}
//
//std::vector<double> vector_add(const std::vector<double>& v1, const std::vector<double>& v2, double alpha) {
//    std::size_t n = v1.size();
//    std::vector<double> result(n);
//    for (std::size_t i = 0; i < n; ++i) {
//        result[i] = v1[i] + alpha * v2[i];
//    }
//    return result;
//}
//
//std::vector<double> vector_subtract(const std::vector<double>& v1, const std::vector<double>& v2) {
//    std::size_t n = v1.size();
//    std::vector<double> result(n);
//    for (std::size_t i = 0; i < n; ++i) {
//        result[i] = v1[i] - v2[i];
//    }
//    return result;
//}
//
//std::vector<double> scalar_vector_multiply(const std::vector<double>& v, double alpha) {
//    std::size_t n = v.size();
//    std::vector<double> result(n);
//    for (std::size_t i = 0; i < n; ++i) {
//        result[i] = v[i] * alpha;
//    }
//    return result;
//}

template <typename T>
T dot_product(const std::vector<T>& v1, const std::vector<T>& v2) {
    return std::inner_product(v1.begin(), v1.end(), v2.begin(), T(0));
}

template <typename T>
std::vector<T> matrix_vector_multiply(const std::vector<std::vector<T>>& A, const std::vector<T>& v) {
    std::size_t n = A.size();
    std::vector<T> result(n, T(0));
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            result[i] += A[i][j] * v[j];
        }
    }
    return result;
}

template <typename T>
std::vector<T> vector_add(const std::vector<T>& v1, const std::vector<T>& v2, T alpha) {
    std::size_t n = v1.size();
    std::vector<T> result(n);
    for (std::size_t i = 0; i < n; ++i) {
        result[i] = v1[i] + alpha * v2[i];
    }
    return result;
}

template <typename T>
std::vector<T> vector_subtract(const std::vector<T>& v1, const std::vector<T>& v2) {
    std::size_t n = v1.size();
    std::vector<T> result(n);
    for (std::size_t i = 0; i < n; ++i) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

template <typename T>
std::vector<T> scalar_vector_multiply(const std::vector<T>& v, T alpha) {
    std::size_t n = v.size();
    std::vector<T> result(n);
    for (std::size_t i = 0; i < n; ++i) {
        result[i] = v[i] * alpha;
    }
    return result;
}

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyLinearSolvingMethod::Solution<T> applyConjugateGradient(MatrixType<T> &A, VectorType<T> &b, const size_t max_iterations, const T tolerance) {

    std::vector<T> x(b.size());
    std::vector<std::vector<T>> matrix(b.size());

    for(size_t row = 0; row < A.getRows(); row++) {
        matrix[row] = A[row];
    }
    std::vector<T> result(b.size(), -10);

    std::vector<T> r = vector_subtract(b.getData(), matrix_vector_multiply(A.getData(), x));
    std::vector<T> p = r;
    T rsold = dot_product(r, r);

    std::size_t i;
    for (i = 0; i < max_iterations; ++i) {
        std::vector<T> Ap = matrix_vector_multiply(A.getData(), p);
        T alpha = rsold / dot_product(p, Ap);
        x = vector_add(x, p, alpha);
        r = vector_subtract(r, scalar_vector_multiply(Ap, alpha));
        T rsnew = dot_product(r, r);

        if (sqrt(rsnew) < tolerance)
            break;

        p = vector_add(r, p, rsnew / rsold);
        rsold = rsnew;
    }
    MyLinearSolvingMethod::Solution<T> solution;
    solution.iterative_error = sqrt(dot_product(r, r));
    solution.iterations = i;
    solution.converged = i < max_iterations;
    solution.x = VectorType<T>(p.size(), 0);
    for(size_t idx = 0; idx < p.size(); idx++) {
        x[idx] = p[idx];
    }
    return solution;
}
//template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
//MyLinearSolvingMethod::Solution<T> applyConjugateGradient(const MatrixType<T> &A, const VectorType<T> &b, const size_t max_iterations, const T tolerance) {
//    const size_t n = A.getRows(); // Assuming A is a square matrix
//    if (A.getCols() != n) {
//        throw std::invalid_argument("Matrix A must be square.");
//    }
//
//    MyLinearSolvingMethod::Solution<T> results(n); // Initialize the results object with the size of the matrix
//
//    VectorType<T> r = b - A * results.x; // Initial residual
//    VectorType<T> p = r;                 // Initial search direction
//    T rsold = r * r;                     // Dot product of residual with itself
//
//    T tolerance_squared = tolerance * tolerance;
//    T alpha, rsnew;
//
//    for (results.iterations = 0; results.iterations < max_iterations; ++(results.iterations)) {
//        VectorType<T> Ap = A * p;
//        alpha = rsold / (p * Ap);
//        VectorType<T> x_new = results.x + p * alpha; // Compute the new solution vector
//        r = r - Ap * alpha;                          // Update the residual
//
//        rsnew = r * r;                               // New dot product of residual with itself
//        if (rsnew < tolerance_squared) {
//            results.converged = true;
//            results.x = x_new;                       // Update the solution vector
//            break; // Convergence achieved
//        }
//
//        p = r + p * (rsnew / rsold);                 // Update the search direction
//        rsold = rsnew;                               // Update the dot product for the next iteration
//        results.x = x_new;                           // Update the solution vector
//    }
//
//    // Calculate the final error using your L2 implementation
//    results.iterative_error = std::sqrt(MyBLAS::L2(results.x, b - A * results.x, n));
//    return results;
//}


} // namespace MyRelaxationMethod
#endif // NE591_008_CONJUGATEGRADIENT_H
