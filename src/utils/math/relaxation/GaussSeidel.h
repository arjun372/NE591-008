/**
 * @file GaussSeidel.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the implementation of the Gauss-Seidel method for solving systems of linear equations.
 */

#ifndef NE591_008_GAUSSSEIDEL_H
#define NE591_008_GAUSSSEIDEL_H

#include "SOR.h"
#include "math/LinearSolver.h"
#include "math/blas/BLAS.h"
#include "math/blas/Matrix.h"
#include "math/blas/Ops.h"
#include "math/blas/Vector.h"

/**
 * @namespace MyRelaxationMethod
 * @brief This namespace contains implementations of various relaxation methods for solving systems of linear equations.
 */
namespace MyRelaxationMethod {

/**
 * @brief This function applies the Gauss-Seidel method to solve a system of linear equations.
 * @tparam T The data type of the elements in the matrix and vectors (e.g., double, float).
 * @param A The matrix of coefficients in the system of equations.
 * @param b The vector of constants in the system of equations.
 * @param max_iterations The maximum number of iterations to perform.
 * @param tolerance The tolerance for convergence. The method stops when the error is less than this value.
 * @return A Solution object containing the solution vector, the number of iterations performed, whether the method
 * converged, and the final error.
 */
template <typename T>
MyLinearSolvingMethod::Solution<T> applyGaussSeidel(const MyBLAS::Matrix<T> &A, const MyBLAS::Vector<T> &b,
                                                    const size_t max_iterations, const T tolerance) {
    const T relaxation_factor = 1;
    return applySOR(A, b, max_iterations, tolerance, relaxation_factor);
}
} // namespace MyRelaxationMethod

#endif // NE591_008_GAUSSSEIDEL_H
