/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief Compute methods for inlab4 in NE591-008.
 */

#pragma once

#include <iostream>
#include <cmath>

#include "utils/CommandLine.h"
#include "utils/Stopwatch.h"
#include "InputsOutputs.h"

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
template<typename T>
myBLAS::Vector forwardSubstitution(const myBLAS::Matrix &L, const myBLAS::Vector &b) {
    const auto n = static_cast<int64_t>(b.size());
    myBLAS::Vector y(n);
    for (int64_t i = 0; i < n; i++) {
        T sum = 0.0f;
        for (int64_t j = 0; j < i; j++) {
            sum += L[i][j] * y[j];
        }
        y[i] = (b[i] - sum) / L[i][i];
    }
    return y;
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
template<typename T>
myBLAS::Vector backwardSubstitution(const myBLAS::Matrix &U, const myBLAS::Vector &y) {
    const auto n = static_cast<int64_t>(y.size());
    myBLAS::Vector x(n);
    for (int64_t i = n - 1; i >= 0; i--) {
        T sum = 0.0f;
        for (int64_t j = i + 1; j < n; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }
    return x;
}
