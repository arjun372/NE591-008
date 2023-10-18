/**
 * @file LUP.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief LU factorization with pivoting
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

#ifndef NE591_008_LUP_H
#define NE591_008_LUP_H

#include <cmath>
#include <iostream>

#include "LU.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

#include "../../CommandLine.h"
#include "../../Stopwatch.h"
#include "Factorize.h"
#include "math/LinearSolver.h"

/**
 * @namespace MyBLAS
 * @brief Namespace containing linear algebra functions and classes.
 */
namespace MyBLAS::LUP {

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
template <template<typename> class MatrixType, typename T>
MatrixType<T> dooLittleFactorizeLUP(MatrixType<T> &L, MatrixType<T> &U, const MatrixType<T> &A) {

    MatrixType pivotedA = A;

    const size_t n = pivotedA.getCols();

    // Initialize the permutation matrix P to the identity matrix
    MatrixType P = MatrixType<T>::eye(n);

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
template <template<typename> class MatrixType, typename T>
MatrixType<T> recursiveFactorizeLUP(MatrixType<T> &L, MatrixType<T> &U, const MatrixType<T> &A) {
    const size_t n = A.getCols();

    MatrixType<T> P = MatrixType<T>::eye(n);

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
        MatrixType<T> A_swapped = A;
        A_swapped.swapRows(0, maxIndex);

        // Divide A into four submatrices
        MatrixType<T> A11 = A_swapped.subMatrix(0, 0, 1, 1);
        MatrixType<T> A12 = A_swapped.subMatrix(0, 1, 1, n - 1);
        MatrixType<T> A21 = A_swapped.subMatrix(1, 0, n - 1, 1);
        MatrixType<T> A22 = A_swapped.subMatrix(1, 1, n - 1, n - 1);

        // Compute the LU factorization of A11
        U[0][0] = A11[0][0];
        L[0][0] = 1;

        // Update A12 and A21
        A12 = (A12 / U[0][0]);
        A21 = (A21 / L[0][0]);

        // Update A22
        A22 = A22 - (A21 * A12);

        // Recursively apply the same procedure to the updated A22
        MatrixType<T> L22(n - 1, n - 1);
        MatrixType<T> U22(n - 1, n - 1);
        P.setSubMatrix(1, 1, recursiveFactorizeLUP(L22, U22, A22));

        // Combine the results
        L.setSubMatrix(0, 0, MatrixType<T>(1, 1, L[0][0]));
        L.setSubMatrix(0, 1, MatrixType<T>(1, n - 1));
        L.setSubMatrix(1, 0, A21);
        L.setSubMatrix(1, 1, L22);

        U.setSubMatrix(0, 0, MatrixType<T>(1, 1, U[0][0]));
        U.setSubMatrix(0, 1, A12);
        U.setSubMatrix(1, 0, MatrixType<T>(n - 1, 1));
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
template <template<typename> class MatrixType, typename T>
MatrixType<T> factorizeLUwithPartialPivoting(MatrixType<T> &L, MatrixType<T> &U,
                                                 const MatrixType<T> &A) {
    const auto n = A.getRows();

    // Initialize L, U, and P
    L = MatrixType<T>(n, n, 0);
    U = A;
    auto P = MatrixType<T>::eye(n);

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

/**
 * @brief This function performs LU factorization on a given matrix A and stores the results in matrices L and U.
 * The function uses the Doolittle's method for LU factorization and is a part of the MyBLAS library.
 *
 * @tparam T This is the data type of the elements in the matrices. It can be any numeric type.
 * @param L This is the output parameter representing the lower triangular matrix after factorization.
 * @param U This is the output parameter representing the upper triangular matrix after factorization.
 * @param A This is the input matrix that is to be factorized. It should be a square matrix.
 * @return The function returns the permutation matrix P from the LU factorization.
 * @note The function does not check if the input matrix A is square. It is the responsibility of the caller to
 * ensure this.
 */
template <template<typename> class MatrixType, typename T>
MatrixType<T> factorize(MatrixType<T> &L, MatrixType<T> &U, const MatrixType<T> &A) {
    return dooLittleFactorizeLUP(L, U, A);
}

/**
 * @brief This function performs LU factorization on a given matrix A and stores the results in matrices L and U.
 * The function uses the Doolittle's method for LU factorization and is a part of the MyBLAS library.
 *
 * @tparam T This is the data type of the elements in the matrices. It can be any numeric type.
 * @param A This is the input matrix that is to be factorized. It should be a square matrix.
 * @return The function returns a Parameters object which contains the lower triangular matrix L, the upper
 * triangular matrix U, and the permutation matrix P from the LU factorization.
 * @note The function does not check if the input matrix A is square. It is the responsibility of the caller to
 * ensure this.
 */
template <template<typename> class MatrixType, typename T>
MyFactorizationMethod::Parameters<T> factorize(const MatrixType<T> &A) {
    MyFactorizationMethod::Parameters<T> parameters(A.getRows());
    parameters.P = dooLittleFactorizeLUP(parameters.L, parameters.U, A);
    return parameters;
}

/**
 * @brief This function solves a system of linear equations Ax = b using LU factorization with partial pivoting.
 *
 * The function first performs LU factorization on the matrix A using the Doolittle's method.
 * It then solves the system Ly = Pb and Ux = y using forward and backward substitution, respectively.
 *
 * @tparam T This is the data type of the elements in the matrices and vectors. It can be any numeric type.
 * @param A This is the input matrix that represents the coefficients of the linear equations. It should be square.
 * @param b This is the input vector that represents the constant terms of the linear equations.
 * @param tolerance This is the tolerance for the forward and backward substitution. The default value is 0.
 * @return The function returns a Solution object which contains the solution vector x.
 * @note The function does not check if the input matrix A is square or if the dimensions of A and b are compatible.
 * It is the responsibility of the caller to ensure this.
 */
template <template<typename> class MatrixType, template<typename> class VectorType, typename T>
MyLinearSolvingMethod::Solution<T> applyLUP(const MatrixType<T> &A, const VectorType<T> &b,
                                            const T tolerance = 0) {

    const size_t n = A.getRows(); // Get the number of rows in the matrix A

    MyFactorizationMethod::Parameters<T> parameters(n);
    parameters.P = dooLittleFactorizeLUP(parameters.L, parameters.U, A);

    MyLinearSolvingMethod::Solution<T> results(n); // Initialize the results object with the size of the matrix

    const auto Pb = parameters.P * b;

    const MyBLAS::Vector<T> y = MyBLAS::forwardSubstitution(parameters.L, Pb);
    const MyBLAS::Vector<T> x = MyBLAS::backwardSubstitution(parameters.U, y);

    results.x = x;

    return results;
}

} // namespace MyBLAS::LUP

#endif // NE591_008_LUP_H
