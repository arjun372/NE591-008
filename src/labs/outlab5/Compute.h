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

template <typename T>
static void factorizeLUP(MyBLAS::Matrix &L, MyBLAS::Matrix &U, MyBLAS::Matrix &A, MyBLAS::Matrix &P) {
    const size_t n = A.getCols();

    // Initialize the permutation matrix P to the identity matrix
    P = MyBLAS::Matrix::eye(n);

    // Perform LU factorization with partial pivoting
    for (size_t i = 0; i < n; i++) {
        // Find the pivot row
        T max_val = 0;
        size_t pivot_row = i;
        for (size_t j = i; j < n; j++) {
            if (std::abs(A[j][i]) > max_val) {
                max_val = std::abs(A[j][i]);
                pivot_row = j;
            }
        }

        // Swap rows in A, L, and P
        if (pivot_row != i) {
            A.swapRows(i, pivot_row);
            L.swapRows(i, pivot_row);
            P.swapRows(i, pivot_row);
        }

        // Perform factorization on the updated matrix A
        MyBLAS::LU::factorize<T>(L, U, A);
    }
}

#endif //NE591_008_OUTLAB_5_COMPUTE_H
