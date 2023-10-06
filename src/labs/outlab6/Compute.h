/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the Compute namespace which includes methods for outlab6 in NE591-008.
 */

#ifndef NE591_008_OUTLAB6_COMPUTE_H
#define NE591_008_OUTLAB6_COMPUTE_H

#include <iostream>
#include <cmath>

#include "math/blas/Matrix.h"
#include "math/factorization/LU.h"
#include "math/factorization/Factorize.h"

#include "math/relaxation/PointJacobi.h"
#include "math/relaxation/GaussSeidel.h"
#include "math/relaxation/SSOR.h"
#include "math/factorization/LUP.h"

/**
 * @namespace Compute
 * @brief This namespace contains methods for performing various computations.
 */
namespace Compute {

    // TODO:: Document
    static void usingLUP(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
        MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
        MyBLAS::Vector<long double> &b = inputs.input.constants;

        if (!MyFactorizationMethod::passesPreChecks(A, b)) {
            std::cerr<<"Aborting LUP calculation\n";
            return;
        }

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyBLAS::LUP::applyLUP(A, b);
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;

    }

    /**
     * @brief This method applies the Point Jacobi relaxation method to the given input matrices.
     * @param outputs A reference to the OutLab6Outputs object where the results will be stored.
     * @param inputs A reference to the OutLab6Inputs object that contains the input matrices.
     */
    static void usingPointJacobi(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
        MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
        MyBLAS::Vector<long double> &b = inputs.input.constants;
        const size_t max_iterations = inputs.input.max_iterations;
        const long double threshold = inputs.input.threshold;

        if (!MyRelaxationMethod::passesPreChecks(A, b)) {
            std::cerr<<"Aborting Point jacobi calculation\n";
            return;
        }

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold);
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;
    }

    // TODO:: DOCUMENT
    static void usingGaussSeidel(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
        MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
        MyBLAS::Vector<long double> &b = inputs.input.constants;
        const size_t max_iterations = inputs.input.max_iterations;
        const long double threshold = inputs.input.threshold;

        if (!MyRelaxationMethod::passesPreChecks(A, b)) {
            std::cerr<<"Aborting Gauss Seidel calculation\n";
            return;
        }

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyRelaxationMethod::applyGaussSeidel<long double>(A, b, max_iterations, threshold);
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;
    }

    // TODO:: DOCUMENT
    static void usingSOR(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
        MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
        MyBLAS::Vector<long double> &b = inputs.input.constants;
        const size_t max_iterations = inputs.input.max_iterations;
        const long double threshold = inputs.input.threshold;
        const long double omega = inputs.input.relaxation_factor;

        if (!MyRelaxationMethod::passesPreChecks(A, b)) {
            std::cerr<<"Aborting SOR calculation\n";
            return;
        }

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyRelaxationMethod::applySOR<long double>(A, b, max_iterations, threshold, omega);
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;
    }

    // TODO:: DOCUMENT
    static void usingJacobiSOR(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
        MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
        MyBLAS::Vector<long double> &b = inputs.input.constants;
        const size_t max_iterations = inputs.input.max_iterations;
        const long double threshold = inputs.input.threshold;
        const long double omega = inputs.input.relaxation_factor;

        if (!MyRelaxationMethod::passesPreChecks(A, b)) {
            std::cerr<<"Aborting SOR point jacobi calculation\n";
            return;
        }

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold, omega);
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;
    }

    // TODO:: DOCUMENT
    static void usingSymmetricSOR(OutLab6Outputs &outputs, OutLab6Inputs &inputs) {
        MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
        MyBLAS::Vector<long double> &b = inputs.input.constants;
        const size_t max_iterations = inputs.input.max_iterations;
        const long double threshold = inputs.input.threshold;
        const long double omega = inputs.input.relaxation_factor;

        if (!MyRelaxationMethod::passesPreChecks(A, b)) {
            std::cerr<<"Aborting symmetric SOR calculation\n";
            return;
        }

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyRelaxationMethod::applySSOR<long double>(A, b, max_iterations, threshold, omega);
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;
    }
}


#endif //NE591_008_OUTLAB6_COMPUTE_H
