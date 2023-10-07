/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the Compute namespace which includes methods for solving linear systems using various
 * methods.
 */

#ifndef NE591_008_OUTLAB6_COMPUTE_H
#define NE591_008_OUTLAB6_COMPUTE_H

#include <cmath>
#include <iostream>

#include "math/blas/Matrix.h"
#include "math/factorization/Factorize.h"
#include "math/factorization/LU.h"

#include "math/factorization/LUP.h"
#include "math/relaxation/GaussSeidel.h"
#include "math/relaxation/PointJacobi.h"
#include "math/relaxation/SSOR.h"

/**
 * @namespace Compute
 * @brief Namespace containing methods for solving linear systems using various methods.
 */
namespace Compute {

/**
 * @brief Solves a linear system using LUP decomposition.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingLUP(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
    MyBLAS::Vector<long double> &b = inputs.input.constants;

    if (!MyFactorizationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting LUP calculation\n";
        return;
    }

    Stopwatch<Nanoseconds> timer;
    timer.restart();
    { outputs.solution = MyBLAS::LUP::applyLUP(A, b); }
    timer.click();

    const auto time = static_cast<long double>(timer.duration().count());
    outputs.execution_time = time;
    outputs.solution.converged = true;
}

/**
 * @brief Solves a linear system using the Point Jacobi method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingPointJacobi(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
    MyBLAS::Vector<long double> &b = inputs.input.constants;
    const size_t max_iterations = inputs.input.max_iterations;
    const long double threshold = inputs.input.threshold;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting Point jacobi calculation\n";
        return;
    }

    Stopwatch<Nanoseconds> timer;
    timer.restart();
    { outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold); }
    timer.click();

    const auto time = static_cast<long double>(timer.duration().count());
    outputs.execution_time = time;
}

/**
 * @brief Solves a linear system using the Gauss-Seidel method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingGaussSeidel(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
    MyBLAS::Vector<long double> &b = inputs.input.constants;
    const size_t max_iterations = inputs.input.max_iterations;
    const long double threshold = inputs.input.threshold;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting Gauss Seidel calculation\n";
        return;
    }

    Stopwatch<Nanoseconds> timer;
    timer.restart();
    { outputs.solution = MyRelaxationMethod::applyGaussSeidel<long double>(A, b, max_iterations, threshold); }
    timer.click();

    const auto time = static_cast<long double>(timer.duration().count());
    outputs.execution_time = time;
}

/**
 * @brief Solves a linear system using the Successive Over-Relaxation (SOR) method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingSOR(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
    MyBLAS::Vector<long double> &b = inputs.input.constants;
    const size_t max_iterations = inputs.input.max_iterations;
    const long double threshold = inputs.input.threshold;
    const long double omega = inputs.input.relaxation_factor;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting SOR calculation\n";
        return;
    }

    Stopwatch<Nanoseconds> timer;
    timer.restart();
    { outputs.solution = MyRelaxationMethod::applySOR<long double>(A, b, max_iterations, threshold, omega); }
    timer.click();

    const auto time = static_cast<long double>(timer.duration().count());
    outputs.execution_time = time;
}

/**
 * @brief Solves a linear system using the Jacobi SOR method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingJacobiSOR(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
    MyBLAS::Vector<long double> &b = inputs.input.constants;
    const size_t max_iterations = inputs.input.max_iterations;
    const long double threshold = inputs.input.threshold;
    const long double omega = inputs.input.relaxation_factor;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting SOR point jacobi calculation\n";
        return;
    }

    Stopwatch<Nanoseconds> timer;
    timer.restart();
    { outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold, omega); }
    timer.click();

    const auto time = static_cast<long double>(timer.duration().count());
    outputs.execution_time = time;
}

/**
 * @brief Solves a linear system using the Symmetric SOR method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingSymmetricSOR(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
    MyBLAS::Vector<long double> &b = inputs.input.constants;
    const size_t max_iterations = inputs.input.max_iterations;
    const long double threshold = inputs.input.threshold;
    const long double omega = inputs.input.relaxation_factor;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting symmetric SOR calculation\n";
        return;
    }

    Stopwatch<Nanoseconds> timer;
    timer.restart();
    { outputs.solution = MyRelaxationMethod::applySSOR<long double>(A, b, max_iterations, threshold, omega); }
    timer.click();

    const auto time = static_cast<long double>(timer.duration().count());
    outputs.execution_time = time;
}
} // namespace Compute

#endif // NE591_008_OUTLAB6_COMPUTE_H
