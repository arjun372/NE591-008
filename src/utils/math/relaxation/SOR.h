/**
 * @file SOR.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the implementation of the Successive Over-Relaxation (SOR) method for solving systems of
 * linear equations.
 */

#ifndef NE591_008_SOR_H
#define NE591_008_SOR_H

#include "math/LinearSolver.h"
#include "math/blas/BLAS.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/Ops.h"
#include "math/blas/vector/Vector.h"

/**
 * @namespace MyRelaxationMethod
 * @brief This namespace contains implementations of various relaxation methods for solving systems of linear equations.
 */
namespace MyRelaxationMethod {

/**
 * @brief This function applies the Successive Over-Relaxation (SOR) method to solve a system of linear equations.
 * @tparam T The data type of the elements in the matrix and vectors (e.g., double, float).
 * @param A The matrix of coefficients in the system of equations.
 * @param b The vector of constants in the system of equations.
 * @param max_iterations The maximum number of iterations to perform.
 * @param tolerance The tolerance for convergence. The method stops when the error is less than this value.
 * @param relaxation_factor The relaxation factor to use in the SOR method. This value is typically between 0 and 2.
 * @return A Solution object containing the solution vector, the number of iterations performed, whether the method
 * converged, and the final error.
 */
template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyLinearSolvingMethod::Solution<T> applySOR(const MatrixType<T> &A, const VectorType<T> &b,
                                            const size_t max_iterations, const T tolerance, const T relaxation_factor) {

    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
    MyLinearSolvingMethod::Solution<T> results(n); // Initialize the results object with the size of the matrix

    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum

    // Start the iteration
    for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {

        VectorType<T> old_x = results.x; // Save the old solution vector

        // For each row in the matrix
        for (size_t row = 0; row < n; row++) {

            // subtract the contribution from the diagonal term, since it should not be counted.
            T sum = -(A[row][row] * results.x[row]);

            // For each column in the matrix, add the product of the matrix element and corresponding element in the
            // solution vector to the sum
            for (size_t col = 0; col < n; col++) {
                sum += A[row][col] * results.x[col];
            }

            // Update the solution vector with the new value, including the relaxation factor
            results.x[row] = (static_cast<T>(1) - relaxation_factor) * old_x[row] + (relaxation_factor / A[row][row]) * (b[row] - sum);
        }

        // Calculate the L2 norm of the difference between the new and old solution vectors
        iterative_error_squared = MyBLAS::L2(results.x, old_x, n);

        // If the squared error is less than the squared tolerance, set the convergence flag to true and break the loop
        if (iterative_error_squared < tolerance_squared) {
            results.converged = true;
            break;
        }
    }

    // Calculate the final error as the square root of the squared error
    results.iterative_error = std::sqrt(iterative_error_squared);
    return results;
}
} // namespace MyRelaxationMethod

#endif // NE591_008_SOR_H
