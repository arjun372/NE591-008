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

    const size_t n = A.getRows();           // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum
    const T tolerance_squared = std::pow(tolerance, 2); // we will compare the squares since this saves us many sqrt ops

    VectorType<T> x(n, 0);          // Initialize guess
    VectorType<T> r = b - (A * x);  // Initial residual
    VectorType<T> p = r;            // Initial search direction

    for (size_t iterations = 0; iterations < max_iterations; iterations++) {

        // place loop-specific variables inside the loop to guide the compiler to know that some parts of the loop
        // calculation are independent

        T r_dot = r * r; // Dot product of the residual with itself
        VectorType<T> Ap = A * p; // Matrix-vector product
        T pAp = p * Ap; // Dot product for the denominator in alpha calculation

        // Check for division by zero
        if (pAp == static_cast<T>(0)) {
            results.iterations = iterations;
            // Handle the error appropriately, e.g., throw an exception or set an error flag
            break;
        }

        T alpha = r_dot / pAp; // Step size
        x = x + alpha * p; // Update the estimate of the solution
        r = r - alpha * Ap; // Update the residual

        iterative_error_squared = r * r; // Calculate the square of the L2 norm of the residual
        if (iterative_error_squared < tolerance_squared) {
            results.converged = true;
            results.iterations = iterations;
            break; // Convergence achieved
        }

        T r_dot_1 = iterative_error_squared; // Dot product of the new residual with itself
        T betaK = r_dot_1 / r_dot; // Calculate the beta coefficient
        p = r + betaK * p; // Update the search direction
    }

    results.iterative_error = std::sqrt(iterative_error_squared);
    if (!results.converged) {
        results.iterations = max_iterations;
    }
    results.x = x;
    return results;
}

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyJacobiPreconditionedConjugateGradient(const MatrixType<T> &A, const VectorType<T> &b, const size_t max_iterations, const T tolerance) {

    const size_t n = A.getRows();           // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum
    const T tolerance_squared = std::pow(tolerance, 2); // we will compare the squares since this saves us many sqrt ops

    VectorType<T> x(n, 0);          // Initialize guess
    VectorType<T> r = b - (A * x);  // Initial residual

    // Create and apply the Jacobi preconditioner
    VectorType<T> M_inv(n); // Vector to store the inverse of the diagonal elements of A
    for (size_t i = 0; i < n; ++i) {
        M_inv[i] = static_cast<T>(1) / A(i, i); // Assuming A(i, i) is never zero
    }

    VectorType<T> z = VectorType<T>::elementwiseMultiply(r, M_inv); // Apply preconditioner: z = M^-1 * r
    VectorType<T> p = z;         // Initial search direction

    for (size_t iterations = 0; iterations < max_iterations; iterations++) {

        T r_dot = r * z; // Dot product of the residual with the preconditioned residual
        VectorType<T> Ap = A * p; // Matrix-vector product
        T pAp = p * Ap; // Dot product for the denominator in alpha calculation

        // Check for division by zero
        if (pAp == static_cast<T>(0)) {
            results.iterations = iterations;
            // Handle the error appropriately, e.g., throw an exception or set an error flag
            break;
        }

        T alpha = r_dot / pAp; // Step size
        x = x + alpha * p; // Update the estimate of the solution
        r = r - alpha * Ap; // Update the residual

        // Apply the preconditioner to the new residual
        z = VectorType<T>::elementwiseMultiply(r, M_inv);

        iterative_error_squared = z * r; // Calculate the square of the L2 norm of the preconditioned residual
        if (iterative_error_squared < tolerance_squared) {
            results.converged = true;
            results.iterations = iterations;
            break; // Convergence achieved
        }

        T betaK = (z * r) / r_dot; // Calculate the beta coefficient using the preconditioned residual
        p = z + betaK * p; // Update the search direction
    }

    results.iterative_error = std::sqrt(iterative_error_squared);
    if (!results.converged) {
        results.iterations = max_iterations;
    }
    results.x = x;
    return results;
}

} // namespace MyRelaxationMethod
#endif // NE591_008_CONJUGATEGRADIENT_H
