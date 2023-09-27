/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief Compute methods for outlab5 in NE591-008.
 */

#ifndef NE591_008_OUTLAB_5_COMPUTE_H
#define NE591_008_OUTLAB_5_COMPUTE_H

#include <iostream>
#include <cmath>

#include "math/blas/Matrix.h"
#include "math/blas/LU.h"

/**
 * LU factorization, also known as lower-upper factorization, is a method used to decompose a square matrix into the product of a lower triangular matrix and an upper triangular matrix. There are several algorithms used for LU factorization, including:

1. Doolittle's Algorithm: This algorithm computes the LU factorization by setting the diagonal elements of the lower triangular matrix to 1 and solving for the remaining elements of the upper and lower triangular matrices.

2. Crout's Algorithm: Similar to Doolittle's algorithm, Crout's algorithm computes the LU factorization by setting the diagonal elements of the upper triangular matrix to 1 and solving for the remaining elements of the upper and lower triangular matrices.

3. Cholesky Decomposition: This algorithm is used for symmetric positive definite matrices. It decomposes the matrix into the product of a lower triangular matrix and its transpose.

4. Partial Pivoting: This technique is used to improve the numerical stability of LU factorization algorithms. It involves swapping rows of the matrix to ensure that the largest element in each column is on the diagonal.

5. Complete Pivoting: This technique is similar to partial pivoting but involves swapping both rows and columns of the matrix to ensure that the largest element in each column is on the diagonal.

6. Block LU Decomposition: This algorithm divides the matrix into smaller blocks and performs LU factorization on each block. This can improve the efficiency of the algorithm, especially for large matrices.

7. Recursive LU Decomposition: This algorithm uses a divide-and-conquer approach to compute the LU factorization of a matrix. It recursively decomposes the matrix into smaller submatrices and computes their LU factorizations.

8. Parallel LU Decomposition: This algorithm is designed to take advantage of parallel computing architectures to perform LU factorization more efficiently. It involves dividing the matrix into smaller blocks and performing LU factorization on each block in parallel.

9. Communication-avoiding LU Decomposition: This algorithm aims to minimize the communication between processors in parallel computing environments, which can be a significant bottleneck in the performance of parallel LU factorization algorithms.

10. Packed Storage LU Decomposition: This algorithm is designed for matrices with a specific structure, such as banded or symmetric matrices. It stores only the non-zero elements of the matrix and performs LU factorization using this packed storage format, which can save memory and improve performance.
 * @tparam T
 * @param L
 * @param U
 * @param A
 * @return
 */

//TODO:: Document
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
                max_val = fabsl(pivotedA[j][i]);
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

//TODO:: Document
template <typename T>
static void recursiveFactorizeLUP(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A, MyBLAS::Matrix<T> &P) {
    const size_t n = A.getCols();

    if (n == 1) {
        U[0][0] = A[0][0];
        L[0][0] = 1;
        P[0][0] = 1;
    } else {
        // Find the index of the maximum element in the first column of A
        size_t maxIndex = 0;
        T maxElement = abs(A[0][0]);
        for (size_t i = 1; i < n; i++) {
            if (abs(A[i][0]) > maxElement) {
                maxElement = abs(A[i][0]);
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
        A12 = A12 / U[0][0];
        A21 = L[0][0] * A21;

        // Update A22
        A22 = A22 - A21 * A12;

        // Recursively apply the same procedure to the updated A22
        MyBLAS::Matrix<T> L22(n - 1, n - 1);
        MyBLAS::Matrix<T> U22(n - 1, n - 1);
        MyBLAS::Matrix<T> P22(n - 1, n - 1);
        recursiveFactorizeLUP(L22, U22, A22, P22);

        // Combine the results
        MyBLAS::Matrix<T> L11(1, 1, L[0][0]);
        L.setSubMatrix(0, 0, L11);
        L.setSubMatrix(0, 1, A12);
        L.setSubMatrix(1, 0, A21);
        L.setSubMatrix(1, 1, L22);

        MyBLAS::Matrix<T> U11(1, 1, U[0][0]);
        U.setSubMatrix(0, 0, U11);
        U.setSubMatrix(0, 1, A12);
        U.setSubMatrix(1, 0, A21);
        U.setSubMatrix(1, 1, U22);

        // Update the permutation matrix
        P = MyBLAS::Matrix<T>::eye(n);
        P.swapRows(0, maxIndex);
        P.setSubMatrix(1, 1, P22);
    }
}


//template <typename T>
//static MyBLAS::Matrix<T> factorizeLUP(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A) {
//    auto P = MyBLAS::Matrix<T>::eye(A.getCols());
//    recursiveFactorizeLUP(L, U, A, P);
//    return P;
//}

/**
 * @brief Perform LU decomposition with Gaussian elimination and partial pivoting.
 *
 * This function performs LU decomposition with Gaussian elimination and partial pivoting on a given matrix (A).
 * The input matrix (A) is decomposed into a lower triangular matrix (L) and an upper triangular matrix (U),
 * such that A = LU. Partial pivoting is used to improve numerical stability.
 *
 * @tparam T Using templates for data type consistency in computation.
 * @param A The input matrix to be decomposed.
 * @param L The resulting lower triangular matrix.
 * @param U The resulting upper triangular matrix.
 * @param P The resulting permutation matrix.
 */
template<typename T>
void luDecompositionWithPartialPivoting(const MyBLAS::Matrix<T> &A, MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, MyBLAS::Matrix<T> &P) {
    const auto n = A.getRows();

    // Initialize L, U, and P
    L = MyBLAS::Matrix<T>(n, n, 0);
    U = A;
    P = MyBLAS::Matrix<T>::eye(n);

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
}

template <typename T>
static MyBLAS::Matrix<T> factorizeLUP(MyBLAS::Matrix<T> &L, MyBLAS::Matrix<T> &U, const MyBLAS::Matrix<T> &A) {

    MyBLAS::Matrix<T> P;
    luDecompositionWithPartialPivoting(A, L, U, P);
    return P;
    // return dooLittleFactorizeLUP(L, U, A);
//    int n = A.getRows();
//
//    // Initialize L, U, and P
//    L = MyBLAS::Matrix<T>(n, n, 0);
//    U = A;
//    MyBLAS::Matrix<T> P = MyBLAS::Matrix<T>::eye(n);
//
//    for (int k = 0; k < n; ++k) {
//        // Find the pivot row
//        int pivot_row = k;
//        for (int i = k + 1; i < n; ++i) {
//            if (std::abs(U[i][k]) > std::abs(U[pivot_row][k])) {
//                pivot_row = i;
//            }
//        }
//
//        // Swap rows in the permutation matrix and U matrix
//        P.swapRows(k, pivot_row);
//        U.swapRows(k, pivot_row);
//
//        // Check for singular matrix
//        if (std::abs(U[k][k]) < 1e-12) {
//            throw std::runtime_error("Singular matrix encountered during LU decomposition with partial pivoting.");
//        }
//
//        // Compute L and U
//        for (int i = k + 1; i < n; ++i) {
//            L[i][k] = U[i][k] / U[k][k];
//            for (int j = k; j < n; ++j) {
//                U[i][j] -= L[i][k] * U[k][j];
//            }
//        }
//    }
//
//    // Set the diagonal of L to 1
//    for (int i = 0; i < n; ++i) {
//        L[i][i] = 1;
//    }
//
//    return P;
}


void lu_decomposition_partial_pivoting(const std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U, std::vector<int>& P) {
    int n = A.size();

    // Initialize L, U, and P
    L.resize(n, std::vector<double>(n, 0));
    U.resize(n, std::vector<double>(n, 0));
    P.resize(n);
    for (int i = 0; i < n; ++i) {
        P[i] = i;
    }

    for (int k = 0; k < n; ++k) {
        // Find the pivot row
        int pivot_row = k;
        for (int i = k + 1; i < n; ++i) {
            if (std::abs(A[P[i]][k]) > std::abs(A[P[pivot_row]][k])) {
                pivot_row = i;
            }
        }

        // Swap rows in the permutation matrix
        std::swap(P[k], P[pivot_row]);

        // Check for singular matrix
        if (std::abs(A[P[k]][k]) < 1e-12) {
            throw std::runtime_error("Singular matrix encountered during LU decomposition with partial pivoting.");
        }

        // Compute L and U
        for (int i = k; i < n; ++i) {
            U[k][i] = A[P[k]][i];
            L[i][k] = A[P[i]][k] / U[k][k];
        }
    }
}

#endif //NE591_008_OUTLAB_5_COMPUTE_H
