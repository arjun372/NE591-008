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

#include "math/relaxation/RelaxationMethods.h"
#include "blas/solver/LinearSolver.h"

/**
 * @namespace MyRelaxationMethod
 * @brief This namespace contains implementations of various relaxation methods for solving systems of linear equations.
 */
namespace MyRelaxationMethod {

#include <cmath>
#include <limits>
#include <stdexcept>

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyConjugateGradient(const MatrixType<T> &A, const VectorType<T> &b, const size_t max_iterations, const T tolerance) {

    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    T iterative_error = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum

    VectorType<T> x(n, -10);                         // Initialize guess
    VectorType<T> b_prime = (A * results.x);
    VectorType<T> r = b - b_prime;                 // Initial residual
    VectorType<T> p = r;                          // Initial search direction

    // these are all loop local
    size_t iterations;
    T r_dot, pAp, alpha, r_dot_1, betaK;
    VectorType<T> Ap, alphaP, betaKp, alphaAp;
    for (iterations = 0; iterations < max_iterations; iterations++) {
        r_dot = r * r;
        Ap = A * p;
        pAp = p * Ap;
        alpha = r_dot / pAp;
        alphaP = alpha * p;

        x = x + alphaP;

        alphaAp = alpha * Ap;
        iterative_error = MyBLAS::L2(r, alphaAp);
        if(iterative_error < tolerance) {
            results.converged = true;
            break;
        }

        r = r - alphaAp;

        r_dot_1 = r * r;
        betaK = r_dot_1 / r_dot;
        betaKp = betaK * p;
        p = r + betaKp;
    }

    results.iterative_error = iterative_error;
    results.iterations = iterations;
    return results;
}

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

//template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
//MyBLAS::Solver::Solution<T> applyConjugateGradient(MatrixType<T> &A, VectorType<T> &b, const size_t max_iterations, const T tolerance) {
//
//    std::vector<T> x(b.size());
//    std::vector<std::vector<T>> matrix(b.size());
//    MyBLAS::Solver::Solution<T> solution;
//
//    for(size_t row = 0; row < A.getRows(); row++) {
//        matrix[row] = A[row];
//    }
//    std::vector<T> result(b.size(), -10);
//
//    std::vector<T> r = vector_subtract(b.getData(), matrix_vector_multiply(A.getData(), x));
//    std::vector<T> p = r;
//    T rsold = dot_product(r, r);
//
//    for (solution.iterations = 0; solution.iterations < max_iterations; ++(solution.iterations)) {
//        std::cout<<rsold<<": "<<solution.iterations<<" of "<<max_iterations<<std::endl;
//        std::vector<T> Ap = matrix_vector_multiply(A.getData(), p);
//        T alpha = rsold / dot_product(p, Ap);
//        x = vector_add(x, p, alpha);
//        r = vector_subtract(r, scalar_vector_multiply(Ap, alpha));
//        T rsnew = dot_product(r, r);
//
//        if (sqrt(static_cast<T>(rsnew)) < tolerance) {
//            solution.converged = true;
//            break;
//        }
//        p = vector_add(r, p, rsnew / rsold);
//        rsold = rsnew;
//    }
//
//    solution.iterative_error = sqrt(static_cast<T>(dot_product(r, r)));
//    solution.x = VectorType<T>(p.size(), 0);
//    for(size_t idx = 0; idx < p.size(); idx++) {
//        x[idx] = p[idx];
//    }
//    return solution;
//}
//template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
//MyBLAS::Solver::Solution<T> applyConjugateGradient(const MatrixType<T> &A, const VectorType<T> &b, const size_t max_iterations, const T tolerance) {
//    const size_t n = A.getRows(); // Assuming A is a square matrix
//    if (A.getCols() != n) {
//        throw std::invalid_argument("Matrix A must be square.");
//    }
//
//    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix
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
