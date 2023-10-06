/**
 * @file SOR.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief TODO:: Document
 * Successive over-relaxation
*/

#ifndef NE591_008_SOR_H
#define NE591_008_SOR_H

#include "math/LinearSolver.h"
#include "math/blas/Ops.h"
#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include "math/blas/MyBLAS.h"

/**
 * @namespace MyRelaxationMethod
 * @brief This namespace contains implementations of various relaxation methods for solving systems of linear equations.
 */
namespace MyRelaxationMethod {

    //TODO:: DOCUMENT
    template<typename T>
    MyLinearSolvingMethod::Solution<T> applySOR(const MyBLAS::Matrix<T> &A, const MyBLAS::Vector<T> &b,
                                                const size_t max_iterations, const T tolerance,
                                                const T relaxation_factor) {

        const size_t n = A.getRows();   // Get the number of rows in the matrix A
        MyLinearSolvingMethod::Solution<T> results(n);    // Initialize the results object with the size of the matrix

        const T tolerance_squared = std::pow(tolerance, 2);        // Calculate the square of the tolerance
        T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum possible value

        // Start the iteration
        for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {
            MyBLAS::Vector<T> old_x = results.x; // Save the old solution vector

            // For each row in the matrix
            for (size_t row = 0; row < n; row++) {
                T sum = 0;
                // For each column in the matrix
                for (size_t col = 0; col < n; col++) {
                    // If it's not the diagonal element
                    if (row != col) {
                        // Add the product of the matrix element and the corresponding element in the solution vector to the sum
                        sum += A[row][col] * results.x[col];
                    }
                }

                // Update the solution vector with the new value, including the relaxation factor
                results.x[row] = (1 - relaxation_factor) * old_x[row] + (relaxation_factor / A[row][row]) * (b[row] - sum);
            }

            // Calculate the L2 norm of the difference between the new and old solution vectors
            iterative_error_squared = MyBLAS::L2Norm<MyBLAS::Vector<T>, T>(results.x, old_x, n);

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
}

#endif //NE591_008_SOR_H
