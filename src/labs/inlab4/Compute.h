/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief Compute methods for inlab4 in NE591-008.
 */

#pragma once

#include <cmath>
#include <iostream>

#include "math/blas/BLAS.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"
namespace InLab04 {
/**
 * @brief Perform forward substitution
 *
 * Function to perform forward substitution, which is used in solving a system of linear equations
 * after the system matrix has been decomposed into a lower triangular matrix and an upper triangular matrix.
 *
 * @tparam T Using templates for data type consistency in computation.
 * @param L A lower triangular matrix.
 * @param b A vector in the system of linear equations Ax = b.
 * @return The result vector after performing forward substitution.
 */
template <typename T> MyBLAS::Vector<T> doForwardSubstitution(const MyBLAS::Matrix<T> &L, const MyBLAS::Vector<T> &b) {
    return MyBLAS::forwardSubstitution<T>(L, b);
}

/**
 * @brief Perform backward substitution
 *
 * Function to perform backward substitution, which is used in solving a system of linear equations
 * after the system matrix has been decomposed into a lower triangular matrix and an upper triangular matrix,
 * and forward substitution has been performed.
 *
 * @tparam T Using templates for data type consistency in computation.
 * @param U An upper triangular matrix.
 * @param y The result vector after performing forward substitution.
 * @return The solution vector after performing backward substitution.
 */
template <typename T> MyBLAS::Vector<T> doBackwardSubstitution(const MyBLAS::Matrix<T> &U, const MyBLAS::Vector<T> &y) {
    return MyBLAS::backwardSubstitution<T>(U, y);
}
} // namespace InLab04
