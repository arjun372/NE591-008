/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief Compute methods for inlab6 in NE591-008.
 */

#ifndef NE591_008_INLAB6_COMPUTE_H
#define NE591_008_INLAB6_COMPUTE_H

#include <iostream>
#include <cmath>

#include "math/blas/Matrix.h"
#include "math/blas/LU.h"

#include "math/relaxation_methods/PointJacobi.h"


// TODO:: DOCUMENT
namespace Compute {

    // TODO:: DOCUMENT
    static void usingPointJacobi(RelaxationMethodOutputs &outputs, InputMatrices & inputs) {
        MyBLAS::Matrix<long double> &A = inputs.coefficients;
        MyBLAS::Vector<long double> &b = inputs.constants;
        const size_t max_iterations = inputs.max_iterations;
        const long double threshold = inputs.threshold;

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            auto solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold);
            outputs.solution = solution;
        }
        timer.click();

        const auto time = static_cast<long double>(timer.duration().count());
        outputs.execution_time = time;
    }

    // TODO:: DOCUMENT
    static void usingGaussSeidel(RelaxationMethodOutputs &outputs) {

    }

    // TODO:: DOCUMENT
    static void usingSOR(RelaxationMethodOutputs &outputs) {

    }

    // TODO:: DOCUMENT
    static void usingSymmetricSOR(RelaxationMethodOutputs &outputs) {

    }
}


#endif //NE591_008_INLAB6_COMPUTE_H
