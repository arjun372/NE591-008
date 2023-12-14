/**
 * @file MyBLAS.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief My BLAS methods for NE591-008.
 *
 */

#ifndef NE591_008_BLAS_H
#define NE591_008_BLAS_H

#include <cmath>

#include "math/blas/Ops.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

namespace MyBLAS {

/**
 * @brief Checks if the given matrix is a square matrix.
 *
 * This function checks if the input matrix has an equal number of rows and columns.
 *
 * @param M The input matrix to be checked.
 * @return true if the input matrix is a square matrix, false otherwise.
 */
template <typename MatrixType> inline bool isSquareMatrix(const MatrixType &M) {
    return M.getRows() > 0 && M.getCols() == M.getRows();
}

/**
 * @brief Checks if the given matrix is empty.
 *
 * This function checks if the input matrix has empty rows or columns.
 *
 * @param M The input matrix to be checked.
 * @return true if the input matrix is empty, false otherwise.
 */
template <typename MatrixType> inline bool isEmptyMatrix(const MatrixType &M) {
    return M.getRows() == 0 || M.getCols() == 0;
}

/**
 * @brief Checks if the given vector is empty.
 *
 * This function checks if the input vector has no items.
 *
 * @param V The input vector to be checked.
 * @return true if the input vector is empty, false otherwise.
 */
template <typename VectorType> inline bool isEmptyVector(const VectorType &V) {
    return V.size() == 0;
}

/**
 * @brief Checks if the given vector is purely a zero vector.
 *
 * @param V The input vector to be checked.
 * @return true if the input vector has only zero values, false otherwise.
 */
template <template<typename> class U, typename T> bool isBinaryVector(const U<T> &V) {
    const size_t items = V.size();
    for (size_t i = 0; i < items; i++) {
        if (V[i] != 0 && V[i] != 1) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks if the given matrix is a binary matrix.
 *
 * This function checks if all elements of the input matrix are either 0 or 1.
 *
 * @param M The input matrix to be checked.
 * @return true if the input matrix is a binary matrix, false otherwise.
 */
template <template<typename> class U, typename T> bool isBinaryMatrix(const U<T> &M) {
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
 * @brief Checks if the given vector is a zero vector.
 *
 * This function checks if all elements of the input vector are 0.
 *
 * @param V The input vector to be checked.
 * @return true if the input vector is a zero vector, false otherwise.
 */
template <template<typename> class U, typename T> bool isZeroVector(const U<T> &V) {
    const size_t items = V.size();
    for (size_t i = 0; i < items; i++) {
        if (V[i] != 0) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks if the given vector is a unit vector.
 *
 * This function checks if all elements of the input vector are 1.
 *
 * @param V The input vector to be checked.
 * @return true if the input vector is a unit vector, false otherwise.
 */
template <template<typename> class U, typename T> bool isUnitVector(const U<T> &V) {
    return norm(V) == norm(V/norm(V));
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
template <template<typename> class U, typename T> bool isUpperTriangularMatrix(const U<T> &M) {
    static_assert(std::is_same<U<T>, MyBLAS::Matrix<T>>::value, "U must be a MyBLAS::Matrix type");
    if (!MyBLAS::isSquareMatrix(M)) {
        return false;
    }

    const size_t cols = M.getCols();
    const size_t rows = M.getRows();

    // Iterate over the rows of the input matrix U
    for (size_t i = 0; i < rows; i++) {
        // Iterate over the cols of the input matrix U
        for (size_t j = 0; j < cols; j++) {
            // NANs and +inf, -inf make the matrix invalid
            if (std::isinf(M[i][j]) || std::isnan(M[i][j])) {
                return false;
            }
            // Lower triangular is always 0
            if (i > j && M[i][j] != 0) {
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
template <template<typename> class U, typename T> bool isUnitLowerTriangularMatrix(const U<T> &L) {
    static_assert(std::is_same<U<T>, MyBLAS::Matrix<T>>::value, "U must be a MyBLAS::Matrix type");
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
template <template<typename> class U, typename T> bool isPermutationMatrix(const U<T> &P) {
    static_assert(std::is_same<U<T>, MyBLAS::Matrix<T>>::value, "U must be a MyBLAS::Matrix type");

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
 * @brief Checks if the given matrix is symmetric.
 *
 * This function checks if the input matrix is equal to its transpose.
 *
 * @param A The input matrix to be checked.
 * @param A For floating-point values, check within a given tolerance
 * @return true if the input matrix is symmetric, false otherwise.
 */
template <template<typename> class M, typename T> bool isSymmetricMatrix(const M<T> &A, const T tolerance = 0) {

    // Check if the matrix is square
    if (!MyBLAS::isSquareMatrix(A)) {
        return false;
    }

    const size_t n = A.getRows();
    if (tolerance == 0) {
        // Check if the matrix is equal to its transpose
        for (size_t i = 0; i < n; i++) {
            for (size_t j = i + 1 ; j < n; j++) {
                if (A[i][j] != A[j][i]) {
                    return false;
                }
            }
        }
    } else {
        const T threshold = std::numeric_limits<T>::epsilon() * tolerance;
        // Check if the matrix is equal to its transpose, within a margin
        for (size_t i = 0; i < n; i++) {
            for (size_t j = i + 1; j < n; j++) {
                if (std::abs(A[i][j] - A[j][i]) > threshold) {
                    return false;
                }
            }
        }
    }

    return true;
}

/**
 * @brief Checks if the given matrix is positive definite using Cholesky decomposition.
 *
 * This function attempts to perform Cholesky decomposition on the input matrix. If the decomposition
 * can be completed without encountering any non-positive elements on the diagonal, the matrix is
 * considered positive definite.
 *
 * @tparam T The data type of the matrix elements.
 * @param A The input matrix to be checked.
 * @return true if the input matrix is positive definite, false otherwise.
 */
template <template<typename> class M, typename T>
bool isPositiveDefiniteMatrix(const M<T> &A) {
    // Check if the matrix is square
    if (!MyBLAS::isSquareMatrix(A)) {
        return false;
    }

    const size_t n = A.getRows();
    M<T> L(n, n, static_cast<T>(0)); // Initialize lower triangular matrix

    bool isPositiveDefinite = true;
    #pragma omp parallel for simd default(none) shared(L, A) reduction(|:isPositiveDefinite)
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            T sum = 0;
            if (j == i) { // Diagonal elements
                for (size_t k = 0; k < j; k++) {
                    sum += L[j][k] * L[j][k];
                }
                L[j][j] = A[j][j] - sum;
                // If the diagonal element is not positive, the matrix is not positive definite
                if (L[j][j] <= static_cast<T>(0)) {
                    isPositiveDefinite = false;
                }
                L[j][j] = std::sqrt(L[j][j]);
            } else { // Off-diagonal elements
                for (size_t k = 0; k < j; ++k) {
                    sum += L[i][k] * L[j][k];
                }
                if (L[j][j] == static_cast<T>(0)) {
                    isPositiveDefinite = false;
                }
                L[i][j] = (A[i][j] - sum) / L[j][j];
            }
        }
    }
    return isPositiveDefinite; // The matrix is positive definite if Cholesky decomposition is successful
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
template <template<typename> class U, typename T>
bool allDiagonalElementsBelowThreshold(const U<T> &A, const T &threshold = 0) {

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
template <template<typename> class U, typename T> bool noZerosInDiagonal(const U<T> &A) {
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
template <template<typename> class U, typename T> bool isDiagonallyDominant(const U<T> &A) {

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
template <typename MatrixType, typename VectorType>
bool haveEqualRank(const MatrixType &A, const VectorType &b) {
    return A.getRows() == b.size();
}

// TODO:: Figure out why this is commented out
///**
// * @brief Global function to multiply a scalar with a matrix or vector.
// * @param scalar Scalar to multiply the matrix or vector with.
// * @param matrix Matrix or vetor to be multiplied.
// * @return Resultant matrix or vector after multiplication.
// */
//template <typename T> MyBLAS::Matrix<T> operator*(const T &scalar, const MyBLAS::Matrix<T> &matrix) {
//    return matrix * scalar;
//}
//
///**
// * @brief Global function to add a scalar to a matrix.
// * @param scalar Scalar to add to the matrix.
// * @param matrix Matrix to be added to.
// * @return Resultant matrix after addition.
// */
//template <typename T> MyBLAS::Matrix<T> operator+(const T &scalar, const MyBLAS::Matrix<T> &matrix) {
//    return matrix + scalar;
//}
//

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

template <template<typename> class M, template<typename> class V, typename T> T matrixWeightedInnerProduct(const V<T> &y, const V<T> &z, const M<T> &A) {
    assert(y.size() == A.getRows() && z.size() == A.getCols());
    return y * (A * z);
}

template <typename T> Vector<T> abs(const Vector<T> v) {
    return Vector<T>(v.size(), [v](size_t i) { return std::abs(v[i]); });
}
} // namespace MyBLAS

#endif // NE591_008_BLAS_H
