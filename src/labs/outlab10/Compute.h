/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the declaration and implementation of the Compute namespace, which provides a function to
 * compute the sum of a series.
 */

#ifndef NE591_008_OUTLAB10_COMPUTE_H
#define NE591_008_OUTLAB10_COMPUTE_H

/**
 * @namespace Compute
 * @brief This namespace provides a function to compute the sum of a series.
 */
#include "math/relaxation/ConjugateGradient.h"
#include "math/relaxation/SOR.h"
namespace Compute {

static void usingConjugateGradient(OutLab10Outputs &outputs, OutLab10Inputs &inputs) {
    auto profiler = Profiler([&inputs, &outputs] {
        MyBLAS::Matrix<long double> &A = inputs.coefficients;
        MyBLAS::Vector<long double> &b = inputs.constants;
        const size_t max_iterations = inputs.max_iterations;
        const long double threshold = inputs.threshold;
        outputs.solution = MyRelaxationMethod::applyConjugateGradient(A, b, max_iterations, threshold);
    },100, 0, "CG Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}

static void usingSOR(OutLab10Outputs &outputs, OutLab10Inputs &inputs) {
    auto profiler = Profiler([&inputs, &outputs] {
                        const MyBLAS::Matrix<long double> &A = inputs.coefficients;
                        const MyBLAS::Vector<long double> &b = inputs.constants;
                        const size_t max_iterations = inputs.max_iterations;
                        const long double threshold = inputs.threshold;
                        const long double omega = 1.0;
                        outputs.solution = MyRelaxationMethod::applySOR(A, b, max_iterations, threshold, omega);
                    },100, 0, "SOR Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}

} // namespace Compute
#endif // NE591_008_OUTLAB10_COMPUTE_H
