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

#include "blas/solver/LinearSolver.h"
#include "factorization/LUP.h"
#include "math/relaxation/RelaxationMethods.h"

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

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyDirectPowerIteration(const MyBLAS::Solver::Parameters<T> &params) {

    const size_t n = params.coefficients.getRows();           // Get the number of rows in the matrix A

    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix
    results.method = METHOD_DIRECT_POWER_ITERATION;

    // Initialize x with a randomly sampled uniform dist between [-1, 1]
    results.x = params.initial_guess;
    results.converged = false;
    results.iterative_error = std::numeric_limits<T>::max(); // Initialize the error as the maximum

    for (results.iterations = 0; results.iterations < params.max_iterations; ++(results.iterations)) {
        // Calculate the matrix-by-vector product A * x
        const VectorType<T> y = params.coefficients * results.x;

        // Calculate the norm of the new vector
        const T norm = std::sqrt(y * y);

        // Normalize and update the vector
        results.x = y * (static_cast<T>(1) / norm);

        // Estimate the eigenvalue using the direct method
        const T new_eigenvalue = norm;

        // compute the change in eigenvalue
        results.eigenvalue_iterative_error = std::abs(new_eigenvalue - results.eigenvalue);

        // Update the eigenvalue
        results.eigenvalue = new_eigenvalue;

        // compute the relative change (by normalizing)
        results.iterative_error = (results.eigenvalue != static_cast<T>(0)) ? results.eigenvalue_iterative_error / std::abs(results.eigenvalue) : std::numeric_limits<T>::max();

        // check for convergence against the set threshold
        if (results.iterative_error < params.convergence_threshold) {
            results.converged = true;
            break;
        }
    }

    // Calculate the residual
    results.residual = params.coefficients * results.x - results.eigenvalue * results.x;

    // Calculate the infinity norm of the residual
    results.residual_infinite_norm = std::abs(*std::max_element(results.residual.begin(), results.residual.end(), [](T a, T b) {
        return std::abs(a) < std::abs(b);
    }));

    return results;
}

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyRayleighQuotientPowerIteration(const MyBLAS::Solver::Parameters<T> &params) {

    const size_t n = params.coefficients.getRows();           // Get the number of rows in the matrix A

    MyBLAS::Solver::Solution<T> results(n); // Initialize the results object with the size of the matrix
    results.method = METHOD_RAYLEIGH_QUOTIENT_POWER_ITERATION;

    // Initialize x with a randomly sampled uniform dist between [-1, 1]
    results.x = params.initial_guess;
    results.converged = false;
    results.iterative_error = std::numeric_limits<T>::max(); // Initialize the error as the maximum

    for (results.iterations = 0; results.iterations < params.max_iterations; ++(results.iterations)) {
        // Calculate the matrix-by-vector product A * x
        const VectorType<T> y = params.coefficients * results.x;

        // Calculate the norm of the new vector
        const T norm = std::sqrt(y * y);

        // Normalize and update the vector
        results.x = y * (static_cast<T>(1) / norm);

        // Estimate the eigenvalue using the Rayleigh Quotient
        const T new_eigenvalue = results.x * (params.coefficients * results.x); // Compute (x)^T * A * x

        // compute the change in eigenvalue
        results.eigenvalue_iterative_error = std::abs(new_eigenvalue - results.eigenvalue);

        // Update the eigenvalue
        results.eigenvalue = new_eigenvalue;

        // compute the relative change (by normalizing)
        results.iterative_error = (results.eigenvalue != static_cast<T>(0)) ? results.eigenvalue_iterative_error / std::abs(results.eigenvalue) : std::numeric_limits<T>::max();

        // check for convergence against the set threshold
        if (results.iterative_error < params.convergence_threshold) {
            results.converged = true;
            break;
        }
    }

    // Calculate the residual
    results.residual = params.coefficients * results.x - results.eigenvalue * results.x;

    // Calculate the infinity norm of the residual
    results.residual_infinite_norm = std::abs(*std::max_element(results.residual.begin(), results.residual.end(), [](T a, T b) {
        return std::abs(a) < std::abs(b);
    }));

    return results;
}

template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyBLAS::Solver::Solution<T> applyInversePowerIteration(const MyBLAS::Solver::Parameters<T> &params) {
    const auto A = params.getCoefficients();
    const size_t n = A.getRows(); // Assuming A is square
    MyBLAS::Solver::Solution<T> results(n);
    results.x = VectorType<T>(n, 1); // Initialize x with ones
    results.eigenvalue = params.getEigenValue();
    results.converged = false;

    MatrixType<T> I = MatrixType<T>::eye(n); // Create an identity matrix of the same size as A


    for (results.iterations = 0; results.iterations < params.max_iterations; ++(results.iterations)) {
        // Shift the matrix A by mu
        MatrixType<T> A_shifted = A - I * results.eigenvalue;

        // Solve (A - mu * I) * y = bk for y
        VectorType<T> y = MyBLAS::LUP::applyLUP(A_shifted, results.x, params.convergence_threshold).x;

        // Normalize y to obtain the next iterate bk+1
        const T norm = std::sqrt(y * y);
        VectorType<T> bk_next = y * (1.0 / norm);

        // Update mu to the Rayleigh quotient of bk+1 with respect to A
        const T mu_next = (bk_next * (A * bk_next)) / (bk_next * bk_next);

        results.x = bk_next;
        results.eigenvalue = mu_next;

        // Check for convergence
        if (std::abs(mu_next - results.eigenvalue) < params.convergence_threshold) {
            results.converged = true;
            break;
        }
    }

    // Calculate the residual
    results.residual = params.coefficients * results.x - results.eigenvalue * results.x;

    // Calculate the infinity norm of the residual
    results.residual_infinite_norm = std::abs(*std::max_element(results.residual.begin(), results.residual.end(), [](T a, T b) {
        return std::abs(a) < std::abs(b);
    }));

    return results;
}

} // namespace MyRelaxationMethod
#endif // NE591_008_POWER_ITERATION_H
