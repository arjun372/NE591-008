/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief Compute methods for outlab4 in NE591-008.
 *
 * This file contains the implementation of the LU factorization algorithm using Doolittle's method.
 * Doolittle's method is an iterative, direct method for LU decomposition, which is suitable for small
 * to moderately sized matrices with relatively well-conditioned properties. It is a straightforward
 * and easy-to-understand algorithm that can be implemented efficiently using simple loops. However,
 * it may not be as numerically stable as other methods, such as Crout's method or Gaussian elimination
 * with partial pivoting, when dealing with ill-conditioned matrices. It also doesn't take advantage of
 * any matrix structure or sparsity, which can be a drawback for certain specialized applications.
 *
 * An alternative approach to LU decomposition is the recursive LU decomposition, also known as
 * divide-and-conquer LU decomposition. This method is more suitable for larger matrices, ill-conditioned
 * matrices, and cases where numerical stability or special structure exploitation is crucial. However,
 * it may require more effort to implement and can be less predictable in terms of performance for very large matrices
 * since deep recursions can cause stack overflows.
 *
 * My intention is to implement LU factorization using Doolittle's method for OutLab 04, and switch over to the
 * divide-and-conquer method for Lab05 since it will require pivoting anyway. This way, I am exposed to more
 * implementations
 */

#pragma once

#include <iostream>
#include <cmath>

#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"

#include "CommandLine.h"
#include "Stopwatch.h"
#include "InputsOutputs.h"

/**
 * @namespace MyBLAS
 * @brief Namespace containing linear algebra functions and classes.
 */
namespace MyBLAS {

    /**
     * @brief Factorizes a given matrix A into lower and upper triangular matrices L,U using Doolittle's method.
     * @tparam T The data type of the matrix elements.
     * @param[out] L The lower triangular matrix.
     * @param[out] U The upper triangular matrix.
     * @param[in] A The input matrix to be factorized.
     *
     * This function implements the LU factorization algorithm using Doolittle's method, which decomposes a given matrix
     * A into a lower triangular matrix L and an upper triangular matrix U such that A = L * U. The algorithm computes
     * the elements of L and U iteratively by eliminating elements in the lower triangle of the matrix A to form L and
     * elements in the upper triangle to form U. The main loop iterates over the columns of the input matrix A, and for
     * each column i, it computes the elements of the i-th row of U and the i-th column of L.
     */
    template <typename T>
    static void doolittleFactorizeLU(MyBLAS::Matrix &L, MyBLAS::Matrix &U, const MyBLAS::Matrix &A) {

        const size_t n = A.getCols();

        // Iterate over the columns of the input matrix A
        for (size_t i = 0; i < n; i++) {
            // Compute the elements of the i-th row of U
            for (size_t k = i; k < n; k++) {
                T sum = 0.0f;
                // Calculate the sum of the product of the corresponding elements of L and U
                for (size_t j = 0; j < i; j++) {
                    sum += (L[i][j] * U[j][k]);
                }
                // Update the element of U by subtracting the sum from the corresponding element of A
                U[i][k] = A[i][k] - sum;
            }

            // Set the diagonal element of L to 1
            L[i][i] = 1;

            // Compute the elements of the i-th column of L
            for (size_t k = i + 1; k < n; k++) {
                T sum = 0.0f;
                // Calculate the sum of the product of the corresponding elements of L and U
                for (size_t j = 0; j < i; j++) {
                    sum += (L[k][j] * U[j][i]);
                }
                // Update the element of L by dividing the difference between the corresponding element of A
                // and the sum by the diagonal element of U
                L[k][i] = (A[k][i] - sum) / U[i][i];
            }
        }
    }

    /**
     * @brief Factorizes a given matrix A into lower and upper triangular matrices L,U.
     * @tparam T The data type of the matrix elements.
     * @param[out] L The lower triangular matrix.
     * @param[out] U The upper triangular matrix.
     * @param[in] A The input matrix to be factorized.
     */
    template <typename T>
    static void factorizeLU(MyBLAS::Matrix &L, MyBLAS::Matrix &U, const MyBLAS::Matrix &A) {
        doolittleFactorizeLU<T>(L, U, A);
    }

    /**
     * @brief Checks if the given matrix is a valid upper triangular matrix.
     *
     * This function checks if the input matrix is square and if all elements
     * below the main diagonal are zero. If these conditions are met, the matrix
     * is considered a valid upper triangular matrix.
     *
     * @param U The input matrix to be checked.
     * @return true if the input matrix is a valid upper triangular matrix, false otherwise.
     */
    static bool isValidUpperTriangularMatrix(const MyBLAS::Matrix &U) {
        const size_t cols = U.getCols();
        const size_t rows = U.getRows();

        // Check if the matrix is square
        if (cols != rows) {
            return false;
        }

        // Iterate over the rows of the input matrix U
        for (size_t i = 0; i < rows; i++) {
            // Iterate over the cols of the input matrix U
            for (size_t j = 0; j < cols; j++) {
                // Lower triangular is always 0
                if (i > j && U[i][j] != 0.0) {
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * @brief Checks if the given matrix is a valid unit lower triangular matrix.
     *
     * This function checks if the input matrix is square, if all elements
     * above the main diagonal are zero, and if all elements on the main diagonal
     * are one. If these conditions are met, the matrix is considered a valid
     * unit lower triangular matrix.
     *
     * @param L The input matrix to be checked.
     * @return true if the input matrix is a valid unit lower triangular matrix, false otherwise.
     */
    static bool isValidUnitLowerTriangularMatrix(const MyBLAS::Matrix &L) {
        const size_t cols = L.getCols();
        const size_t rows = L.getRows();

        // Check if the matrix is square
        if (cols != rows) {
            return false;
        }

        // Iterate over the rows of the input matrix L
        for (size_t i = 0; i < rows; i++) {
            // Iterate over the cols of the input matrix L
            for (size_t j = 0; j < cols; j++) {
                // leading diagonal is always 1
                if (i == j && L[i][j] != 1.0) {
                    return false;
                }
                // upper triangular is always 0
                if(i < j && L[i][j] != 0.0) {
                    return false;
                }
            }
        }
        return true;
    }
}
