/**
 * @file PointJacobi.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the implementation of the Point Jacobi method for solving systems of linear equations.
 */

#ifndef NE591_008_SORPJ_H
#define NE591_008_SORPJ_H

#include <omp.h>
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"
#include <cstddef>

#include "math/blas/Ops.h"

#include "math/relaxation/RelaxationMethods.h"
#include "utils/math/blas/solver/LinearSolver.h"

/**
 * @namespace MyRelaxationMethod
 * @brief This namespace contains implementations of various relaxation methods for solving systems of linear equations.
 */
namespace MyRelaxationMethod {

/**
 * The Jacobi method requires the matrix `A` to be diagonally dominant, or else it may not converge to the correct
 * solution. A diagonally dominant matrix is one where the absolute value of each diagonal element is greater than
 * the sum of the absolute values of the other elements in the same row.
 *
 * @brief This function applies the Point Jacobi method to solve a system of linear equations.
 * @tparam T The data type of the matrix and vectors (usually float or double).
 * @param A The matrix in the system of equations.
 * @param b The right-hand side vector in the system of equations.
 * @param max_iterations The maximum number of iterations to perform.
 * @param tolerance The tolerance for convergence. The method stops if the L2 norm of the difference between two
 *        successive solutions is less than this value.
 * @param relaxation_factor The purpose of the relaxation factor in the Jacobi method is the same as in the SOR
 *        method: it can potentially accelerate the convergence of the method. The relaxation factor is a weight
 *        that is applied to the new estimate of the solution at each iteration. If the relaxation factor is less
 *        than 1, the new estimate is "dampened", or reduced, which can improve stability but may slow convergence.
 *        If the relaxation factor is greater than 1, the new estimate is "amplified", or increased, which can speed
 *        up convergence but may also cause the method to diverge if the factor is too large.
 *
 * @return A Solution object containing the solution vector, the number of iterations performed, whether the method
 * converged, and the final error.
 * @note This function only works with square matrices. If a non-square matrix is provided, the function will return
 * immediately with an empty solution.
 * @note The function uses the L2 norm to check for convergence. If the system does not converge within the specified
 * number of iterations, the function will return the current solution and set the 'converged' flag to false.
 */
template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyPointJacobi(const MatrixType<T> &A, const VectorType<T> &b,
                                                    const size_t max_iterations, const T tolerance,
                                                    const T relaxation_factor = 1, const size_t threads = 1) {

    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum

    VectorType<T> new_x(n, 0); // Initialize a new vector for the updated solution

    // Start the Point-Jacobi iteration
    for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {
//        std::cout<<results.iterations<<" of "<<max_iterations<<" :: "<<iterative_error_squared<<" : "<<tolerance_squared<<std::endl;
        // Update the solution vector with the new values for this iteration step
        results.x = new_x;

        // If the squared error is less than the squared tolerance, set the convergence flag to true and break the loop
        if (iterative_error_squared < tolerance_squared) {
            results.converged = true;
            break;
        }

        #pragma omp parallel for num_threads(threads) default(none) shared(results, new_x, A, b)
        for (size_t row = 0; row < n; row++) {
            // subtract the contribution from the diagonal term, since it should not be counted.
            T sum = -(A[row][row] * results.x[row]);

            // For each column in the matrix, add the product of the matrix element and corresponding element in the
            // solution vector to the sum
            for (size_t col = 0; col < n; col++) {
                sum += A[row][col] * results.x[col];
            }
            // Update the solution vector with the new value
            new_x[row] = (static_cast<T>(1) - relaxation_factor) * results.x[row] + (relaxation_factor / A[row][row]) * (b[row] - sum);
        }

        iterative_error_squared = MyBLAS::L2(new_x, results.x, n);
    }

    // Calculate the final error as the square root of the squared error
    results.iterative_error = std::sqrt(iterative_error_squared);
    return results;
}

} // namespace MyRelaxationMethod
#endif // NE591_008_SORPJ_H
