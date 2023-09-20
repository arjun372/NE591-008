/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief Compute methods for outlab4 in NE591-008.
 */

#pragma once

#include <iostream>
#include <cmath>

#include "utils/math/blas/Matrix.h"
#include "utils/math/blas/Vector.h"

#include "utils/CommandLine.h"
#include "utils/Stopwatch.h"
#include "InputsOutputs.h"


// TODO:: Document!
namespace MyBLAS {

    //TODO:: Document!
    template <typename T>
    static void factorizeLU(MyBLAS::Matrix &L, MyBLAS::Matrix &U, const MyBLAS::Matrix &A) {
        const size_t n = A.getCols();

        for (size_t i = 0; i < n; i++) {
            for (size_t k = i; k < n; k++) {
                T sum = 0.0f;
                for (size_t j = 0; j < i; j++) {
                    sum += (L[i][j] * U[j][k]);
                }
                U[i][k] = A[i][k] - sum;
            }

            for (size_t k = i; k < n; k++) {
                if (i == k)
                    L[i][i] = 1;
                 {
                    T sum = 0.0f;
                    for (size_t j = 0; j < i; j++) {
                        sum += (L[k][j] * U[j][i]);
                    }
                    L[k][i] = (A[k][i] - sum) / U[i][i];
                }
            }
        }
    }
}
