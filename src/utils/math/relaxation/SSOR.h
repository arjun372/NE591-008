/**
 * @file SSOR.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief TODO:: Document
 * Symmetric successive over-relaxation
*/

#ifndef NE591_008_SSOR_H
#define NE591_008_SSOR_H

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
    MyLinearSolvingMethod::Solution<T> applySSOR(const MyBLAS::Matrix<T> &A, const MyBLAS::Vector<T> &b,
                                                const size_t max_iterations, const T tolerance,
                                                const T relaxation_factor) {

        const size_t n = A.getRows();   // Get the number of rows in the matrix A
        MyLinearSolvingMethod::Solution<T> results(n);    // Initialize the results object with the size of the matrix

        const T tolerance_squared = std::pow(tolerance, 2);        // Calculate the square of the tolerance
        T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum possible value

        // Start the iteration
        for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {
            MyBLAS::Vector<T> old_x = results.x; // Save the old solution vector

            // Forward sweep (like weighted Gauss-Seidel)
            for (size_t row = 0; row < n; row++) {
                T sum = 0;
                for (size_t col = 0; col < n; col++) {
                    if (row != col) {
                        sum += A[row][col] * results.x[col];
                    }
                }
                results.x[row] = (1 - relaxation_factor) * old_x[row] + (relaxation_factor / A[row][row]) * (b[row] - sum);
            }

            // Backward sweep (like Gauss-Seidel, but in reverse order)
            for (size_t rowPlusOne = n; rowPlusOne > 0; rowPlusOne--) {
                T sum = 0;
                const size_t row = rowPlusOne - 1;
                for (size_t col = 0; col < n; col++) {
                    if (row != col) {
                        sum += A[row][col] * results.x[col];
                    }
                }
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

#endif //NE591_008_SSOR_H
