/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the Compute namespace which includes methods for inlab6 in NE591-008.
 */

#ifndef NE591_008_INLAB6_COMPUTE_H
#define NE591_008_INLAB6_COMPUTE_H

#include <cmath>
#include <iostream>

#include "math/blas/Matrix.h"
#include "math/factorization/LU.h"

#include "math/relaxation/PointJacobi.h"

/**
 * @namespace Compute
 * @brief This namespace contains methods for performing various computations.
 */
namespace Compute {

/**
 * @brief This method applies the Point Jacobi relaxation method to the given input matrices.
 * @param outputs A reference to the RelaxationMethodOutputs object where the results will be stored.
 * @param inputs A reference to the InputMatrices object that contains the input matrices.
 */
static void usingPointJacobi(InLab6Outputs &outputs, InLab6Inputs &inputs) {
    MyBLAS::Matrix<long double> &A = inputs.coefficients;
    MyBLAS::Vector<long double> &b = inputs.constants;
    const size_t max_iterations = inputs.max_iterations;
    const long double threshold = inputs.threshold;

    Stopwatch<Nanoseconds> timer;
    timer.restart();
    { outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold); }
    timer.click();

    const auto time = static_cast<long double>(timer.duration().count());
    outputs.execution_time = time;
}

/**
 * @brief This method is a placeholder for the Gauss-Seidel relaxation method.
 * @param outputs A reference to the RelaxationMethodOutputs object where the results will be stored.
 */
static void usingGaussSeidel(InLab6Outputs &outputs) {}

/**
 * @brief This method is a placeholder for the Successive Over-Relaxation (SOR) method.
 * @param outputs A reference to the RelaxationMethodOutputs object where the results will be stored.
 */
static void usingSOR(InLab6Outputs &outputs) {}

/**
 * @brief This method is a placeholder for the Symmetric Successive Over-Relaxation (SSOR) method.
 * @param outputs A reference to the RelaxationMethodOutputs object where the results will be stored.
 */
static void usingSymmetricSOR(InLab6Outputs &outputs) {}
} // namespace Compute

#endif // NE591_008_INLAB6_COMPUTE_H
