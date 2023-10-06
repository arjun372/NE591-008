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
#include "math/blas/LU.h"

#include "math/relaxation_methods/PointJacobi.h"

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
    static void usingPointJacobi(OutLab6Outputs &outputs, OutLab6Inputs & inputs) {
        MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
        MyBLAS::Vector<long double> &b = inputs.input.constants;
        const size_t max_iterations = inputs.input.max_iterations;
        const long double threshold = inputs.input.threshold;

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold);
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;
    }

    /**
     * @brief This method is a placeholder for the Gauss-Seidel relaxation method.
     * @param outputs A reference to the RelaxationMethodOutputs object where the results will be stored.
     */
    static void usingGaussSeidel(OutLab6Outputs &outputs) {

    }

    /**
     * @brief This method is a placeholder for the Successive Over-Relaxation (SOR) method.
     * @param outputs A reference to the RelaxationMethodOutputs object where the results will be stored.
     */
    static void usingSOR(OutLab6Outputs &outputs) {

    }

    /**
     * @brief This method is a placeholder for the Symmetric Successive Over-Relaxation (SSOR) method.
     * @param outputs A reference to the RelaxationMethodOutputs object where the results will be stored.
     */
    static void usingSymmetricSOR(OutLab6Outputs &outputs) {

    }
}


#endif //NE591_008_OUTLAB6_COMPUTE_H
