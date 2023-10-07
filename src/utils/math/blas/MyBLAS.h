/**
 * @file MyBLAS.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief My BLAS methods for NE591-008.
 *
 */

#ifndef NE591_008_MYBLAS_H
#define NE591_008_MYBLAS_H

#include <cmath>

#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"

namespace MyBLAS {

/**
 * @brief Calculates the absolute value of the vector.
 * @return Vector with the absolute value of each element.
 */
template <typename T> Vector<T> abs(const Vector<T> &a) {
    Vector<T> result(a.size(), 0);
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = fabsl(a[i]);
    }
    return result;
}

/**
 * @brief Calculates the absolute value of the matrix.
 * @return Matrix with the absolute value of each element.
 */
template <typename T> Matrix<T> abs(const Matrix<T> &a) {
    Matrix<T> result(a.getRows(), a.getCols(), 0);
    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < a.getCols(); ++j) {
            result[i][j] = fabsl(a[i][j]);
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
template <typename T> inline bool isSquareMatrix(const MyBLAS::Matrix<T> &M) { return M.getCols() == M.getRows(); }

/**
 * @brief Checks if the given matrix is a binary matrix.
 *
 * This function checks if all elements of the input matrix are either 0 or 1.
 *
 * @param M The input matrix to be checked.
 * @return true if the input matrix is a binary matrix, false otherwise.
 */
template <typename T> bool isBinaryMatrix(const MyBLAS::Matrix<T> &M) {
    const size_t rows = M.getRows();
    const size_t cols = M.getCols();

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (M[i][j] != 0 && M[i][j] != 1) {
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
template <typename T> bool isUpperTriangularMatrix(const MyBLAS::Matrix<T> &U) {
    if (!MyBLAS::isSquareMatrix(U)) {
        return false;
    }

    const size_t cols = U.getCols();
    const size_t rows = U.getRows();

    // Iterate over the rows of the input matrix U
    for (size_t i = 0; i < rows; i++) {
        // Iterate over the cols of the input matrix U
        for (size_t j = 0; j < cols; j++) {
            // NANs and +inf, -inf make the matrix invalid
            if (std::isinf(U[i][j]) || std::isnan(U[i][j])) {
                return false;
            }
            // Lower triangular is always 0
            if (i > j && U[i][j] != 0) {
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
template <typename T> bool isUnitLowerTriangularMatrix(const MyBLAS::Matrix<T> &L) {

    if (!MyBLAS::isSquareMatrix(L)) {
        return false;
    }

    const size_t cols = L.getCols();
    const size_t rows = L.getRows();

    // Iterate over the rows of the input matrix L
    for (size_t i = 0; i < rows; i++) {
        // Iterate over the cols of the input matrix L
        for (size_t j = 0; j < cols; j++) {
            // NANs and +inf, -inf make the matrix invalid
            if (std::isinf(L[i][j]) || std::isnan(L[i][j])) {
                return false;
            }
            // leading diagonal is always 1
            if (i == j && L[i][j] != 1) {
                return false;
            }
            // upper triangular is always 0
            if (i < j && L[i][j] != 0) {
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
template <typename T> bool isPermutationMatrix(const MyBLAS::Matrix<T> &P) {

    if (!MyBLAS::isSquareMatrix(P)) {
        return false;
    }

    if (!MyBLAS::isBinaryMatrix(P)) {
        return false;
    }

    const size_t rows = P.getRows();
    const size_t cols = P.getCols();

    // Initialize row and column counters
    std::vector<size_t> rowOnesCount(rows, 0);
    std::vector<size_t> colOnesCount(cols, 0);

    // Iterate over the matrix and update the counters
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {

            // NANs and +inf, -inf make the matrix invalid
            if (std::isinf(P[i][j]) || std::isnan(P[i][j])) {
                return false;
            }

            if (P[i][j] == 1) {
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

/**
 * @brief Checks if all diagonal elements of a matrix are below a certain threshold.
 *
 * This function iterates over the diagonal elements of the input matrix A and checks if their absolute values are
 * below a given threshold.If any diagonal element is not below the threshold, the function returns false. The
 * function also checks for invalid values (NANs and infinities).
 *
 * @tparam T The data type of the matrix elements.
 * @param A The input matrix to be checked.
 * @param threshold The threshold value to compare against. Default is 0.
 * @return true if all diagonal elements are below the threshold, false otherwise.
 */
template <typename T> bool allDiagonalElementsBelowThreshold(const MyBLAS::Matrix<T> &A, T threshold = 0) {

    const size_t n = A.getRows(); // or A.getCols(), since A is a square matrix
    const T absolute_threshold = std::abs(threshold);

    // Iterate over the diagonal elements of the input matrix A
    for (size_t i = 0; i < n; i++) {
        // Check if the diagonal element is zero
        // NANs and +inf, -inf make the matrix invalid
        if (std::isinf(A[i][i]) || std::isnan(A[i][i])) {
            return false;
        }

        if (std::abs(A[i][i]) > absolute_threshold) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks if there are no zeros in the diagonal of a matrix.
 *
 * This function uses the allDiagonalElementsBelowThreshold function to check if there are no zeros in the diagonal
 * of the input matrix A.
 *
 * @tparam T The data type of the matrix elements.
 * @param A The input matrix to be checked.
 * @return true if there are no zeros in the diagonal, false otherwise.
 */
template <typename T> bool noZerosInDiagonal(const MyBLAS::Matrix<T> &A) {
    return !MyBLAS::allDiagonalElementsBelowThreshold(A, static_cast<T>(0));
}

/**
 * @brief Checks if the given matrix is diagonally dominant.
 *
 * This function checks if the input matrix is square, and if the absolute value of each diagonal element
 * is greater than the sum of the absolute values of the other elements in the same row. If these conditions are met,
 * the matrix is considered diagonally dominant.
 *
 * @param A The input matrix to be checked.
 * @return true if the matrix is diagonally dominant, false otherwise.
 */
template <typename T> bool isDiagonallyDominant(const MyBLAS::Matrix<T> &A) {

    if (!MyBLAS::isSquareMatrix(A)) {
        return false;
    }

    const size_t n = A.getRows(); // or A.getCols(), since A is a square matrix

    // Iterate over the rows of the input matrix A
    for (size_t i = 0; i < n; i++) {
        T sum = 0;
        // Iterate over the columns of the input matrix A
        for (size_t j = 0; j < n; j++) {
            // Skip the diagonal element
            if (i != j) {
                sum += std::abs(A[i][j]);
            }
        }
        // Check if the absolute value of the diagonal element is less than or equal to the sum
        if (std::abs(A[i][i]) <= sum) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Makes the given matrix diagonally dominant.
 *
 * This function modifies the input matrix such that the absolute value of each diagonal element
 * is greater than the sum of the absolute values of the other elements in the same row.
 *
 * @param A The input matrix to be modified.
 * @param dominance_offset The offset to be added to the diagonal elements to make the matrix diagonally dominant.
 */
template <typename T> void makeDiagonallyDominant(MyBLAS::Matrix<T> &A, const T dominance_offset = 1) {

    const size_t n = A.getRows(); // or A.getCols(), since A is a square matrix

    // Iterate over the rows of the input matrix A
    for (size_t row = 0; row < n; row++) {
        T nonDiagonalSum = 0;
        // Iterate over the columns of the input matrix A
        for (size_t col = 0; col < n; col++) {
            nonDiagonalSum += std::abs(A[row][col]);
        }
        // get the diagonal value
        const T leadingDiagonalMagnitude = std::abs(A[row][row]);
        nonDiagonalSum -= leadingDiagonalMagnitude; // Subtract the diagonal element

        if (leadingDiagonalMagnitude <= nonDiagonalSum) {
            const T sign = (leadingDiagonalMagnitude == A[row][row]) ? 1 : -1; // retain the sign
            A[row][row] = sign * (nonDiagonalSum + dominance_offset);
        }
    }
}

/**
 * @brief Checks if two matrices have the same rank.
 *
 * This function compares the number of rows in matrix A with the size of vector b to determine if they have the
 * same rank.
 *
 * @tparam T The data type of the matrix and vector elements.
 * @param A The input matrix to be checked.
 * @param b The input vector to be checked.
 * @return true if the matrix and vector have the same rank, false otherwise.
 */
template <typename T> bool haveEqualRank(const MyBLAS::Matrix<T> &A, const MyBLAS::Vector<T> &b) {
    return A.getRows() == b.size();
}

/**
 * @brief Global function to multiply a scalar with a matrix.
 * @param scalar Scalar to multiply the matrix with.
 * @param matrix Matrix to be multiplied.
 * @return Resultant matrix after multiplication.
 */
template <typename T> MyBLAS::Matrix<T> operator*(const T &scalar, const MyBLAS::Matrix<T> &matrix) {
    return matrix * scalar;
}

/**
 * @brief Global function to add a scalar to a matrix.
 * @param scalar Scalar to add to the matrix.
 * @param matrix Matrix to be added to.
 * @return Resultant matrix after addition.
 */
template <typename T> MyBLAS::Matrix<T> operator+(const T &scalar, const MyBLAS::Matrix<T> &matrix) {
    return matrix + scalar;
}

/**
 * @brief Global function to multiply a scalar with a vector.
 * @param scalar Scalar to multiply the matrix with.
 * @param matrix Vector to be multiplied.
 * @return Resultant matrix after multiplication.
 */
template <typename T> MyBLAS::Vector<T> operator*(const T &scalar, const MyBLAS::Vector<T> &vector) {
    return vector * scalar;
}

/**
 * @brief Global function to add a scalar to a vector.
 * @param scalar Scalar to add to the matrix.
 * @param matrix Vector to be added to.
 * @return Resultant matrix after addition.
 */
template <typename T> MyBLAS::Vector<T> operator+(const T &scalar, const MyBLAS::Vector<T> &vector) {
    return vector + scalar;
}

/**
 * @brief Computes the inner product of two matrices.
 * @param rhs Matrix to compute the inner product with.
 * @return The inner product of the two matrices.
 */
template <typename T> MyBLAS::Matrix<T> innerProduct(const MyBLAS::Matrix<T> &A, const MyBLAS::Matrix<T> &B) {
    if (A.getRows() != B.getRows() || A.getCols() != B.getCols()) {
        throw std::invalid_argument("Error: Matrices must have the same dimensions for inner product.");
    }
    MyBLAS::Matrix<T> result(A.getRows(), B.getCols());
    for (size_t row = 0; row < A.getRows(); row++) {
        for (size_t col = 0; col < B.getCols(); col++) {
            result[row][col] = A[row][col] * B[row][col];
        }
    }
    return result;
}
} // namespace MyBLAS

#endif // NE591_008_MYBLAS_H
