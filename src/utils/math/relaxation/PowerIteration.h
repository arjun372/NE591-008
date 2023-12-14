/**
 * @file PowerIteration.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the implementation of the power iterations for solving systems of linear equations.
 */

#ifndef NE591_008_POWER_ITERATION_H
#define NE591_008_POWER_ITERATION_H

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
MyBLAS::Solver::Solution<T> applyPowerIteration(const MatrixType<T> &A, const size_t max_iterations, const T tolerance, const size_t seed = 372) {

    const size_t n = A.getRows();           // Get the number of rows in the matrix A

    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix

    // Initialize x with a randomly sampled uniform dist between [-1, 1]
    results.x = Random::generate_vector(n, static_cast<T>(-1), static_cast<T>(1), seed);
    results.eigenvalue = 0;
    results.converged = false;
    results.iterative_error = std::numeric_limits<T>::max(); // Initialize the  error as the maximum

    T norm;

    for (results.iterations = 0; results.iterations < max_iterations; ++(results.iterations)) {
        // Calculate the matrix-by-vector product A * b_k
        VectorType<T> b_k1 = A * results.x;

        // Calculate the norm of the new vector
        norm = std::sqrt(b_k1 * b_k1);

        // Normalize the vector
        b_k1 = b_k1 * (static_cast<T>(1) / norm);

        // Estimate the eigenvalue using the Rayleigh Quotient
        // T new_eigenvalue_1 = results.x * b_k1;
        T new_eigenvalue = results.x * (A * results.x);  // Compute (bk)^T * A * bk

        // Check for convergence
        results.iterative_error = std::abs(new_eigenvalue - results.eigenvalue);

//        // After normalizing b_k1
//        // Replace the convergence check with the new criterion
//        results.iterative_error = MyBLAS::AbsoluteMaxResidual(b_k1, results.x);

        results.eigenvalue = new_eigenvalue;
        results.x = b_k1;

        if (results.iterative_error < tolerance) {
            results.converged = true;
            break;
        }
    }

    return results;
}

} // namespace MyRelaxationMethod
#endif // NE591_008_POWER_ITERATION_H
