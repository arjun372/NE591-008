/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief Compute methods for outlab5 in NE591-008.
 */

#ifndef NE591_008_OUTLAB_5_COMPUTE_H
#define NE591_008_OUTLAB_5_COMPUTE_H

#include <cmath>
#include <iostream>

#include "math/blas/Matrix.h"
#include "math/factorization/LU.h"

/**
 * @brief Performs Doolittle's LU factorization with partial pivoting on a given matrix.
 *
 * This method first finds the pivot row by identifying the maximum absolute value in each column.
 * It then swaps the pivot row with the current row in both the matrix and the permutation matrix.
 * Finally, it performs Doolittle's LU factorization on the pivoted matrix.
 *
 * @tparam T The data type of the matrix elements.
 * @param L The lower triangular matrix after factorization.
 * @param U The upper triangular matrix after factorization.
 * @param A The input matrix to be factorized.
 * @return The permutation matrix.
 */
template <typename T>
static MyBLAS::Matrix<T> dooLittleFactorizeLUP(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A) {

    MyBLAS::Matrix pivotedA = A;

    const size_t n = pivotedA.getCols();

    // Initialize the permutation matrix P to the identity matrix
    MyBLAS::Matrix P = MyBLAS::Matrix<T>::eye(n);

    // Perform LU factorization with partial pivoting
    for (size_t i = 0; i < n; i++) {
        // Find the pivot row
        T max_val = 0;
        size_t pivot_row = i;
        for (size_t j = i; j < n; j++) {
            if (fabsl(pivotedA[j][i]) > max_val) {
                max_val = std::abs(pivotedA[j][i]);
                pivot_row = j;
            }
        }

        // Swap rows in A and P
        if (pivot_row != i) {
            pivotedA.swapRows(i, pivot_row);
            P.swapRows(i, pivot_row);
        }
    }
    // Perform factorization on the updated matrix A
    MyBLAS::LU::doolittleFactorize<T>(L, U, pivotedA);

    return P;
}

/**
 * @brief Performs recursive LU factorization with partial pivoting on a given matrix.
 *
 * This method first finds the maximum absolute value in the first column of the matrix and swaps the corresponding row
 * with the first row.It then divides the matrix into four submatrices and performs LU factorization on the top-left
 * submatrix. The method then updates the other submatrices and recursively applies the same procedure to the
 * bottom-right submatrix. Finally, it combines the results into the final L and U matrices.
 *
 * @tparam T The data type of the matrix elements.
 * @param L The lower triangular matrix after factorization.
 * @param U The upper triangular matrix after factorization.
 * @param A The input matrix to be factorized.
 * @return The permutation matrix.
 */
template <typename T>
static MyBLAS::Matrix<T> recursiveFactorizeLUP(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A) {
    const size_t n = A.getCols();

    MyBLAS::Matrix<T> P = MyBLAS::Matrix<T>::eye(n);

    if (n == 1) {
        U[0][0] = A[0][0];
        L[0][0] = 1;
        P[0][0] = 1;
    } else {
        // Find the index of the maximum element in the first column of A
        size_t maxIndex = 0;
        T maxElement = std::abs(A[0][0]);
        for (size_t i = 1; i < n; i++) {
            if (std::abs(A[i][0]) > maxElement) {
                maxElement = std::abs(A[i][0]);
                maxIndex = i;
            }
        }

        // Swap the first row of A with the row containing the maximum element
        MyBLAS::Matrix<T> A_swapped = A;
        A_swapped.swapRows(0, maxIndex);

        // Divide A into four submatrices
        MyBLAS::Matrix<T> A11 = A_swapped.subMatrix(0, 0, 1, 1);
        MyBLAS::Matrix<T> A12 = A_swapped.subMatrix(0, 1, 1, n - 1);
        MyBLAS::Matrix<T> A21 = A_swapped.subMatrix(1, 0, n - 1, 1);
        MyBLAS::Matrix<T> A22 = A_swapped.subMatrix(1, 1, n - 1, n - 1);

        // Compute the LU factorization of A11
        U[0][0] = A11[0][0];
        L[0][0] = 1;

        // Update A12 and A21
        A12 = (A12 / U[0][0]);
        A21 = (A21 / L[0][0]);

        // Update A22
        A22 = A22 - (A21 * A12);

        // Recursively apply the same procedure to the updated A22
        MyBLAS::Matrix<T> L22(n - 1, n - 1);
        MyBLAS::Matrix<T> U22(n - 1, n - 1);
        P.setSubMatrix(1, 1, recursiveFactorizeLUP(L22, U22, A22));

        // Combine the results
        L.setSubMatrix(0, 0, MyBLAS::Matrix<T>(1, 1, L[0][0]));
        L.setSubMatrix(0, 1, MyBLAS::Matrix<T>(1, n - 1));
        L.setSubMatrix(1, 0, A21);
        L.setSubMatrix(1, 1, L22);

        U.setSubMatrix(0, 0, MyBLAS::Matrix<T>(1, 1, U[0][0]));
        U.setSubMatrix(0, 1, A12);
        U.setSubMatrix(1, 0, MyBLAS::Matrix<T>(n - 1, 1));
        U.setSubMatrix(1, 1, U22);
    }

    return P;
}

/**
 * @brief Performs LU factorization with partial pivoting on a given matrix.
 *
 * This method first initializes the L, U, and P matrices.
 * It then finds the pivot element in each column of the U matrix and swaps the corresponding row with the current row
 * in the U, L, and P matrices. The method then performs Gaussian elimination to update the U matrix and calculates the
 * corresponding elements in the L matrix. Finally, it sets the diagonal elements of the L matrix to 1.
 *
 * @tparam T The data type of the matrix elements.
 * @param L The lower triangular matrix after factorization.
 * @param U The upper triangular matrix after factorization.
 * @param A The input matrix to be factorized.
 * @return The permutation matrix.
 */
template <typename T>
static MyBLAS::Matrix<T> factorizeLUwithPartialPivoting(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U,
                                                        const MyBLAS::Matrix<T> &A) {
    const auto n = A.getRows();

    // Initialize L, U, and P
    L = MyBLAS::Matrix<T>(n, n, 0);
    U = A;
    auto P = MyBLAS::Matrix<T>::eye(n);

    for (size_t k = 0; k < n; k++) {
        // Find the pivot element
        T maxVal = 0;
        size_t maxIdx = k;
        for (size_t i = k; i < n; i++) {
            if (std::abs(U[i][k]) > maxVal) {
                maxVal = std::abs(U[i][k]);
                maxIdx = i;
            }
        }

        // Swap rows in U, L, and P
        U.swapRows(k, maxIdx);
        P.swapRows(k, maxIdx);
        if (k > 0) {
            L.swapRows(k, maxIdx);
        }

        // Gaussian elimination
        for (size_t i = k + 1; i < n; i++) {
            L[i][k] = U[i][k] / U[k][k];
            for (size_t j = k; j < n; j++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        // Set the diagonal of L to 1
        L[k][k] = 1;
    }

    return P;
}

// template <typename T>
// static MyBLAS::Matrix<T> factorizeLUP(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A) {
//     return dooLittleFactorizeLUP(L, U, A);
// }

#endif // NE591_008_OUTLAB_5_COMPUTE_H
