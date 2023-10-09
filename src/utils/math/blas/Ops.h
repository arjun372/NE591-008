/**
 * @file Ops.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the definition of the L2Norm function, which calculates the L2 norm (Euclidean distance)
 * between two containers.
 */

#ifndef NE591_008_OPS_H
#define NE591_008_OPS_H

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <random>

namespace MyBLAS {

// TODO:: DOCUMENT
template <typename ContainerType, typename DataType>
inline DataType L2(const ContainerType &a, const ContainerType &b, const size_t n) {
    DataType norm = 0;
    for (size_t i = 0; i < n; i++) {
        const auto difference = a[i] - b[i];
        const auto squared = std::pow(difference, static_cast<DataType>(2));
        norm += squared;
    }
    return norm;
}

// TODO:: DOCUMENT
template <typename T> T L2(const MyBLAS::Vector<T> &a, const MyBLAS::Vector<T> &b) {
    const size_t n = std::min(a.size(), b.size());
    return MyBLAS::L2<MyBLAS::Vector<T>, T>(a, b, n);
}

/**
 * @brief Calculates the L2 norm (Euclidean distance) between two containers.
 *
 * For real numbers, the L2 norm is defined as sqrt(sum(v[i] * v[i])).
 * For complex numbers, the L2 norm is defined as sqrt(sum(v[i] * conj(v[i]))).
 *
 * @tparam ContainerType The type of the containers. This should be a container type that supports indexing with the []
 * operator.
 * @tparam DataType The type of the data contained in the containers. This should be a type that supports subtraction
 * and multiplication.
 *
 * @param a The first container.
 * @param b The second container.
 * @param n The size of the containers. This function assumes that both containers have the same size.
 *
 * @return The L2 norm (Euclidean distance) between the two containers.
 */
template <typename ContainerType, typename DataType>
inline DataType L2Norm(const ContainerType &a, const ContainerType &b, const size_t n) {
    return std::sqrt(L2<ContainerType, DataType>(a, b, n));
}

/**
 * @brief Overloaded function that calculates the L2 norm (Euclidean distance) between two MyBLAS::Vector objects.
 *
 * This function uses the L2Norm function defined above to calculate the L2 norm.
 * The size of the vectors is determined by the smaller of the two vectors.
 *
 * @tparam T The type of the data contained in the vectors. This should be a type that supports subtraction and
 * multiplication.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The L2 norm (Euclidean distance) between the two vectors.
 */
template <typename T> T L2Norm(const MyBLAS::Vector<T> &a, const MyBLAS::Vector<T> &b) {
    const size_t n = std::min(a.size(), b.size());
    return MyBLAS::L2Norm<MyBLAS::Vector<T>, T>(a, b, n);
}

// TODO:: DOCUMENT
/**
 *     ||v|| = sqrt(v1^2 + v2^2 + ... + vn^2)
 * @tparam T
 * @param v
 * @return
 */
template <typename T>
T norm(const MyBLAS::Vector<T>& v) {
    T sum = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        sum += std::pow(v[i], static_cast<T>(2));
    }
    return std::sqrt(sum);
}

/**
 * @brief Calculates the absolute maximum residual between two containers.
 *
 * The absolute maximum residual is the maximum absolute difference between the corresponding elements of the two
 * containers.
 *
 * @tparam ContainerType The type of the containers. This should be a container type that supports indexing with the []
 * operator.
 * @tparam DataType The type of the data contained in the containers. This should be a type that supports subtraction
 * and the std::abs function.
 *
 * @param a The first container.
 * @param b The second container.
 * @param n The size of the containers. This function assumes that both containers have the same size.
 *
 * @return The absolute maximum residual between the two containers.
 */
template <typename ContainerType, typename DataType>
inline DataType AbsoluteMaxResidual(const ContainerType &a, const ContainerType &b, const size_t n) {
    DataType max_residual = 0;
    for (size_t i = 0; i < n; i++) {
        DataType residual = std::abs(a[i] - b[i]);
        max_residual = std::max(max_residual, residual);
    }
    return max_residual;
}

/**
 * @brief Overloaded function that calculates the absolute maximum residual between two MyBLAS::Vector objects.
 *
 * This function uses the AbsoluteMaxResidual function defined above to calculate the absolute maximum residual.
 * The size of the vectors is determined by the smaller of the two vectors.
 *
 * @tparam T The type of the data contained in the vectors. This should be a type that supports subtraction and
 * the std::abs function.
 *
 * @param a The first vector.
 * @param b The second vector.
 *
 * @return The absolute maximum residual between the two vectors.
 */
template <typename T> T AbsoluteMaxResidual(const MyBLAS::Vector<T> &a, const MyBLAS::Vector<T> &b) {
    const size_t n = std::min(a.size(), b.size());
    return MyBLAS::AbsoluteMaxResidual<MyBLAS::Vector<T>, T>(a, b, n);
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

        if (leadingDiagonalMagnitude < nonDiagonalSum) {
            const T sign = (leadingDiagonalMagnitude == A[row][row]) ? 1 : -1; // retain the sign
            A[row][row] = sign * (nonDiagonalSum + dominance_offset);
        }
    }
}
} // namespace MyBLAS
#endif // NE591_008_OPS_H
