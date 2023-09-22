/**
 * @file MyBLAS.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief My BLAS methods for NE591-008.
 *
 * This file contains the implementation of forward and backward substitution algorithms, which are part of the LU
 * decomposition method for solving a system of linear equations. The LU decomposition method involves decomposing a
 * given matrix (A) into a lower triangular matrix (L) and an upper triangular matrix (U), such that A = LU. Then,
 * forward elimination (forward substitution) and back substitution (backward substitution) are performed to solve the
 * system of linear equations.
 *
 * There are other approaches to performing forward and backward elimination, such as:
 *
 * 1. Gaussian Elimination: Involves performing row operations on an augmented matrix [A|b] to transform it into an
 *                          upper triangular matrix. Then, back substitution is performed to solve the system of
 *                          linear equations. Computational complexity: O(n^3). Numerical stability: Partial pivoting
 *                          can be used to improve stability.
 *
 * 2. Gauss-Jordan Elimination: An extension of Gaussian elimination. Involves performing row operations on an
 *                              augmented matrix [A|b] to transform it into a reduced row echelon form (RREF). The
 *                              resulting matrix directly provides the solution to the system of linear equations.
 *                              Computational complexity: O(n^3). Numerical stability: Similar to Gaussian elimination,
 *                              partial pivoting can be used to improve stability.
 *
 * 3. Cholesky Decomposition: Applicable to symmetric positive-definite matrices. Involves decomposing a given matrix
 *                            (A) into a lower triangular matrix (L) and its transpose (L^T), such that A = LL^T. Then,
 *                            forward and backward substitution are performed to solve the system of linear equations.
 *                            Computational complexity: O(n^3/3). Numerical stability: Stable for symmetric
 *                            positive-definite matrices.
 *
 * 4. QR Decomposition: Decomposes a given matrix (A) into an orthogonal matrix (Q) and an upper triangular
 *                      matrix (R), such that A = QR. Then, the system of linear equations is transformed into
 *                      Rx = Q^Tb, and back substitution is performed to solve the transformed system.
 *                      Computational complexity: O(n^3). Numerical stability: Stable, especially when using Householder
 *                      reflections or Gram-Schmidt orthogonalization.
 *
 * 5. Singular Value Decomposition (SVD): Decomposes a given matrix (A) into three matrices: an orthogonal matrix (U), a
 *                                        diagonal matrix (Σ), and another orthogonal matrix (V^T), such that A = UΣV^T.
 *                                        Then, the system of linear equations is transformed into a new system, and the
 *                                        solution is obtained using the inverse of the diagonal matrix. Computational
 *                                        complexity: O(n^3). Numerical stability: Stable, but computationally expensive.
 */
#pragma once

#include <iostream>
#include <cmath>

#include "utils/math/blas/Matrix.h"
#include "utils/math/blas/Vector.h"

#include "utils/CommandLine.h"
#include "utils/Stopwatch.h"
#include "InputsOutputs.h"


namespace MyBLAS {
/**
 * @brief Perform forward substitution
 *
 * This function performs forward substitution, which is used in solving a system of linear equations
 * after the system matrix has been decomposed into a lower triangular matrix (L) and an upper triangular matrix (U).
 * The forward substitution algorithm iterates through each row of the lower triangular matrix (L) and computes
 * the corresponding element in the intermediate result vector (y) by subtracting the sum of the product of the
 * current row elements and the corresponding elements in the intermediate result vector (y) from the corresponding
 * element in the input vector (b).
 *
 * @tparam T Using templates for data type consistency in computation.
 * @param L A lower triangular matrix.
 * @param b A vector in the system of linear equations Ax = b.
 * @return The result vector after performing forward substitution.
 */
    template<typename T>
    MyBLAS::Vector forwardSubstitution(const MyBLAS::Matrix &L, const MyBLAS::Vector &b) {
        const auto n = b.size();
        MyBLAS::Vector y(n);
        for (size_t row = 0; row < n; row++) {
            T sum = 0.0f;
            for (size_t col = 0; col < row; col++) {
                sum += L[row][col] * y[col];
            }
            y[row] = (b[row] - sum);
        }
        return y;
    }

/**
 * @brief Perform backward substitution
 *
 * This function performs backward substitution, which is used in solving a system of linear equations
 * after the system matrix has been decomposed into a lower triangular matrix (L) and an upper triangular matrix (U),
 * and forward substitution has been performed. The backward substitution algorithm iterates through each row of the
 * upper triangular matrix (U) in reverse order and computes the corresponding element in the solution vector (x) by
 * subtracting the sum of the product of the current row elements and the corresponding elements in the solution vector
 * (x) from the corresponding element in the intermediate result vector (y) and then dividing by the diagonal element of
 * the current row in the upper triangular matrix (U).
 *
 * @tparam T Using templates for data type consistency in computation.
 * @param U An upper triangular matrix.
 * @param y The result vector after performing forward substitution.
 * @return The solution vector after performing backward substitution.
 */
    template<typename T>
    MyBLAS::Vector backwardSubstitution(const MyBLAS::Matrix &U, const MyBLAS::Vector &y) {
        const auto n = static_cast<int64_t>(y.size());
        MyBLAS::Vector x(n);
        for (int64_t i = n - 1; i >= 0; i--) {
            T sum = 0.0f;
            for (int64_t j = i + 1; j < n; j++) {
                sum += U[i][j] * x[j];
            }
            x[i] = (y[i] - sum) / U[i][i];
        }
        return x;
    }

    /**
     * @brief Calculates the absolute value of the vector.
     * @return Vector with the absolute value of each element.
     */
    Vector abs(const Vector& a) {
        Vector result(a.size(), 0);
        for (size_t i = 0; i < a.size(); ++i) {
            result[i] = std::abs(a[i]);
        }
        return result;
    }

    /**
    * @brief Calculates the absolute value of the matrix.
    * @return Matrix with the absolute value of each element.
    */
    Matrix abs(const Matrix& a) {
        Matrix result(a.getRows(), a.getCols(), 0);
        for (size_t i = 0; i < a.getRows(); ++i) {
            for (size_t j = 0; j < a.getCols(); ++j) {
                result[i][j] = std::abs(a[i][j]);
            }
        }
        return result;
    }

}
