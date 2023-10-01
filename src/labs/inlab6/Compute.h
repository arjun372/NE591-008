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
    static void usingPointJacobi(RelaxationMethodOutputs &outputs) {
        MyBLAS::Matrix<long double> &A = outputs.inputs.coefficients;
        MyBLAS::Vector<long double> &b = outputs.inputs.constants;
        const size_t max_iterations = outputs.inputs.max_iterations;
        const long double threshold = outputs.inputs.threshold;

        Stopwatch<Nanoseconds> timer;
        timer.restart();
        {
            outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold);
        }
        timer.click();

        outputs.execution_time = static_cast<long double>(timer.duration().count());
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
