/**
 * @file SOR.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the implementation of the Successive Over-Relaxation (SOR) method for solving systems of
 * linear equations.
 */

#ifndef NE591_008_SOR_H
#define NE591_008_SOR_H

#include "math/blas/BLAS.h"
#include "math/blas/Ops.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"
#include "utils/math/blas/solver/LinearSolver.h"

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
MyBLAS::Solver::Solution<T> applySORSerial(const MatrixType<T> &A, const VectorType<T> &b,
                                            const size_t max_iterations, const T tolerance, const T relaxation_factor = 1, const size_t threads = 1) {

    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum

    // Start the iteration
    for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {

        VectorType<T> old_x = results.x; // Save the old solution vector

        // If the squared error is less than the squared tolerance, set the convergence flag to true and break the loop
        if (iterative_error_squared < tolerance_squared) {
            results.converged = true;
            break;
        }

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
    }

    // Calculate the final error as the square root of the squared error
    results.iterative_error = std::sqrt(iterative_error_squared);
    return results;
}

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applySOR(const MatrixType<T> &A, const VectorType<T> &b,
                                             const size_t max_iterations, const T tolerance,
                                             const T relaxation_factor = 1, const size_t threads = 1) {
    const size_t n = A.getRows(); // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum

    // Start the iteration
    for (results.iterations = 0; results.iterations < max_iterations; ++(results.iterations)) {
        VectorType<T> old_x = results.x; // Save the old solution vector

        // If the squared error is less than the squared tolerance, set the convergence flag to true and break the loop
        if (iterative_error_squared < tolerance_squared) {
            results.converged = true;
            break;
        }

        // Update red elements
        #pragma omp parallel for num_threads(threads) default(none) shared(results, A, b, old_x)
        for (size_t row = 0; row < n; row++) {
            T sum = T();
            for (size_t col = 0; col < n; col++) {
                if ((row + col) % 2 == 0) { // Red elements when row+col is even
                    sum += A[row][col] * old_x[col];
                }
            }
            if ((row + row) % 2 == 0) { // Update only red elements
                results.x[row] = (static_cast<T>(1) - relaxation_factor) * old_x[row] +
                                 (relaxation_factor / A[row][row]) * (b[row] - sum + A[row][row] * old_x[row]);
            }
        }

        // Update black elements
        #pragma omp parallel for num_threads(threads) default(none) shared(results, A, b, old_x)
        for (size_t row = 0; row < n; row++) {
            T sum = T();
            for (size_t col = 0; col < n; col++) {
                if ((row + col) % 2 == 1) { // Black elements when row+col is odd
                    sum += A[row][col] * results.x[col]; // Use updated red elements
                }
            }
            if ((row + row) % 2 == 1) { // Update only black elements
                results.x[row] = (static_cast<T>(1) - relaxation_factor) * old_x[row] +
                                 (relaxation_factor / A[row][row]) * (b[row] - sum + A[row][row] * old_x[row]);
            }
        }

        // Calculate the L2 norm of the difference between the new and old solution vectors
        iterative_error_squared = MyBLAS::L2(results.x, old_x, n);
    }

    // Calculate the final error as the square root of the squared error
    results.iterative_error = std::sqrt(iterative_error_squared);
    return results;
}

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applySORBlockParallel(const MatrixType<T> &A, const VectorType<T> &b,
                                             const size_t max_iterations, const T tolerance,
                                             const T relaxation_factor = 1, const size_t threads = 1) {

    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n);        // Initialize the results object with the size of the matrix

    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
    T iterative_error_squared = std::numeric_limits<T>::max();          // Initialize the squared error as the maximum

    // Start the iteration
    for (results.iterations = 0; results.iterations < max_iterations; ++(results.iterations)) {

        //std::cout<<results.iterations<<" of "<<max_iterations<<" :: "<<iterative_error_squared<<" : "<<tolerance_squared<<std::endl;
        VectorType<T> old_x = results.x; // Save the old solution vector

        // If the squared error is less than the squared tolerance, set the convergence flag to true and break the loop
        if (iterative_error_squared < tolerance_squared) {
            results.converged = true;
            break;
        }

        // Parallel block-wise update
        #pragma omp parallel num_threads(threads) default(none) shared(A, b, results, old_x)
        {
            #pragma omp for schedule(static)
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
        }

        // Calculate the L2 norm of the difference between the new and old solution vectors
        iterative_error_squared = MyBLAS::L2(results.x, old_x, n);
    }

    // Calculate the final error as the square root of the squared error
    results.iterative_error = std::sqrt(iterative_error_squared);
    return results;
}

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyPSOR(const MatrixType<T> &A, const VectorType<T> &b,
                                            const size_t max_iterations, const T tolerance, const T relaxation_factor = 1) {

    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum

    // Start the iteration
    for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {

        VectorType<T> old_x = results.x; // Save the old solution vector

// Update red points
#pragma omp parallel for schedule(static)
        for (size_t row = 0; row < n; row++) {
            if ((row % 2) == 0) { // Red points: row index is even
                T sum = -(A[row][row] * results.x[row]);
                for (size_t col = 0; col < n; col++) {
                    sum += A[row][col] * results.x[col];
                }
                results.x[row] = (static_cast<T>(1) - relaxation_factor) * old_x[row] + (relaxation_factor / A[row][row]) * (b[row] - sum);
            }
        }

#pragma omp barrier

// Update black points
#pragma omp parallel for schedule(static)
        for (size_t row = 0; row < n; row++) {
            if ((row % 2) == 1) { // Black points: row index is odd
                T sum = -(A[row][row] * results.x[row]);
                for (size_t col = 0; col < n; col++) {
                    sum += A[row][col] * results.x[col];
                }
                results.x[row] = (static_cast<T>(1) - relaxation_factor) * old_x[row] + (relaxation_factor / A[row][row]) * (b[row] - sum);
            }
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

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyP2SOR(const MatrixType<T> &A, const VectorType<T> &b,
                                            const size_t max_iterations, const T tolerance, const T relaxation_factor = 1) {

    const size_t n = A.getRows();                  // Get the number of rows in the matrix A
    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    const T tolerance_squared = std::pow(tolerance, static_cast<T>(2)); // Calculate the square of the tolerance
    T iterative_error_squared = std::numeric_limits<T>::max(); // Initialize the squared error as the maximum

    // Start the iteration
    for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {

        VectorType<T> old_x = results.x; // Save the old solution vector

// Update the "red" points
#pragma omp parallel for
        for (size_t row = 0; row < n; row += 2) { // Assuming red points are even-indexed
            T sum = 0;
            for (size_t col = 0; col < n; col++) {
                if (col != row) {
                    sum += A[row][col] * old_x[col];
                }
            }
            results.x[row] = (static_cast<T>(1) - relaxation_factor) * old_x[row] + (relaxation_factor / A[row][row]) * (b[row] - sum);
        }

// Synchronize after updating red points
#pragma omp barrier

// Update the "black" points
#pragma omp parallel for
        for (size_t row = 1; row < n; row += 2) { // Assuming black points are odd-indexed
            T sum = 0;
            for (size_t col = 0; col < n; col++) {
                if (col != row) {
                    sum += A[row][col] * results.x[col]; // Use the updated red values
                }
            }
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
