/**
 * @file LU.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief LU factorization and related methods for MyBLAS library
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

#ifndef NE591_008_LU_H
#define NE591_008_LU_H

#include <cmath>
#include <iostream>

#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

#include "../../CommandLine.h"
#include "../../Stopwatch.h"

/**
 * @namespace MyBLAS
 * @brief Namespace containing linear algebra functions and classes.
 */
namespace MyBLAS::LU {

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
static void doolittleFactorize(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A) {

    const size_t n = A.getCols();

    // Iterate over the columns of the input matrix A
    for (size_t i = 0; i < n; i++) {
        // Compute the elements of the i-th row of U
        for (size_t k = i; k < n; k++) {
            T sum = 0;
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
            T sum = 0;
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
 * @brief Performs recursive LU factorization on a given matrix.
 *
 * This function performs LU factorization on a given matrix A and stores the results in matrices L and U.
 * The function uses a recursive divide-and-conquer approach, dividing the matrix into four submatrices and
 * recursively applying the same procedure to the updated submatrix A22.
 *
 * @tparam T The data type of the elements in the matrices.
 * @param L The lower triangular matrix resulting from the LU factorization.
 * @param U The upper triangular matrix resulting from the LU factorization.
 * @param A The input matrix to be factorized.
 */
template <typename T>
static void recursiveLUFactorize(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A) {
    const size_t n = A.getCols();

    if (n == 1) {
        U[0][0] = A[0][0];
        L[0][0] = 1;
        return;
    }

    // Divide A into four submatrices
    MyBLAS::Matrix<T> A11 = A.subMatrix(0, 0, 1, 1);
    MyBLAS::Matrix<T> A12 = A.subMatrix(0, 1, 1, n - 1);
    MyBLAS::Matrix<T> A21 = A.subMatrix(1, 0, n - 1, 1);
    MyBLAS::Matrix<T> A22 = A.subMatrix(1, 1, n - 1, n - 1);

    // Compute the LU factorization of A11
    U[0][0] = A11[0][0];
    L[0][0] = 1;

    // Update A12 and A21
    A12 = A12 / U[0][0];
    A21 = L[0][0] * A21;

    // Update A22
    A22 = A22 - A21 * A12;

    // Recursively apply the same procedure to the updated A22
    MyBLAS::Matrix<T> L22(n - 1, n - 1);
    MyBLAS::Matrix<T> U22(n - 1, n - 1);
    recursiveLUFactorize(L22, U22, A22);

    // Combine the results
    Matrix<T> L11(1, 1, L[0][0]);
    L.setSubMatrix(0, 0, L11);
    L.setSubMatrix(0, 1, A12);
    L.setSubMatrix(1, 0, A21);
    L.setSubMatrix(1, 1, L22);

    Matrix<T> U11(1, 1, U[0][0]);
    U.setSubMatrix(0, 0, U11);
    U.setSubMatrix(0, 1, A12);
    U.setSubMatrix(1, 0, A21);
    U.setSubMatrix(1, 1, U22);
}

/**
 * @brief Factorizes a given matrix A into lower and upper triangular matrices L,U.
 * @tparam T The data type of the matrix elements.
 * @param[out] L The lower triangular matrix.
 * @param[out] U The upper triangular matrix.
 * @param[in] A The input matrix to be factorized.
 */
template <typename T> static void factorize(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, Matrix<T> A) {
    doolittleFactorize<T>(L, U, A);
    // recursiveLUFactorize(L, U, A);
}
} // namespace MyBLAS::LU

#endif // NE591_008_LU_H
