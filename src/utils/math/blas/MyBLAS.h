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

#ifndef NE591_008_MYBLAS_H
#define NE591_008_MYBLAS_H

#include <cmath>

#include "Matrix.h"
#include "Vector.h"

namespace MyBLAS {

    /**
     * @brief Calculates the absolute value of the vector.
     * @return Vector with the absolute value of each element.
     */
    static Vector abs(const Vector& a) {
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
    static Matrix abs(const Matrix& a) {
        Matrix result(a.getRows(), a.getCols(), 0);
        for (size_t i = 0; i < a.getRows(); ++i) {
            for (size_t j = 0; j < a.getCols(); ++j) {
                result[i][j] = std::abs(a[i][j]);
            }
        }
        return result;
    }

    /**
     * @brief Checks if the given matrix is a square matrix.
     *
     * This function checks if the input matrix has an equal number of rows and columns.
     *
     * @param M The input matrix to be checked.
     * @return true if the input matrix is a square matrix, false otherwise.
     */
    static inline bool isSquareMatrix(const MyBLAS::Matrix &M) {
        return M.getCols() == M.getRows();
    }

    /**
     * @brief Checks if the given matrix is a binary matrix.
     *
     * This function checks if all elements of the input matrix are either 0 or 1.
     *
     * @param M The input matrix to be checked.
     * @return true if the input matrix is a binary matrix, false otherwise.
     */
    static bool isBinaryMatrix(const MyBLAS::Matrix &M) {
        const size_t rows = M.getRows();
        const size_t cols = M.getCols();

        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (M[i][j] != 0.0 && M[i][j] != 1.0) {
                    return false;
                }
            }
        }
        return true;
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
    static bool isUpperTriangularMatrix(const MyBLAS::Matrix &U) {
        if(!MyBLAS::isSquareMatrix(U)) {
            return false;
        }

        const size_t cols = U.getCols();
        const size_t rows = U.getRows();

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
    static bool isUnitLowerTriangularMatrix(const MyBLAS::Matrix &L) {

        if(!MyBLAS::isSquareMatrix(L)) {
            return false;
        }

        const size_t cols = L.getCols();
        const size_t rows = L.getRows();

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

    /**
     * @brief Checks if the given matrix is a valid permutation matrix.
     *
     * This function checks if the input matrix is square, binary, and if each row
     * and each column has exactly one element equal to 1.
     *
     * @param P The input matrix to be checked.
     * @return true if the input matrix is a valid permutation matrix, false otherwise.
     */
    static bool isPermutationMatrix(const MyBLAS::Matrix &P) {

        if (!MyBLAS::isSquareMatrix(P)) {
            return false;
        }

        if (!MyBLAS::isBinaryMatrix(P)) {
            return false;
        }

        const size_t rows = P.getRows();
        const size_t cols = P.getCols();

        // Initialize row and column counters
        std::vector<int> rowOnesCount(rows, 0);
        std::vector<int> colOnesCount(cols, 0);

        // Iterate over the matrix and update the counters
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (P[i][j] == 1.0) {
                    rowOnesCount[i]++;
                    colOnesCount[j]++;
                }
            }
        }

        // Check if each row and each column has exactly one element equal to 1
        for (size_t i = 0; i < rows; i++) {
            if (rowOnesCount[i] != 1 || colOnesCount[i] != 1) {
                return false;
            }
        }

        return true;
    }
}

#endif //NE591_008_MYBLAS_H
