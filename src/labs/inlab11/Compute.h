/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the declaration and implementation of the Compute namespace, which provides a function to
 * compute the sum of a series.
 */

#ifndef NE591_008_INLAB11_COMPUTE_H
#define NE591_008_INLAB11_COMPUTE_H

/**
 * @namespace Compute
 * @brief This namespace provides a function to compute the sum of a series.
 */
#include "math/relaxation/ConjugateGradient.h"
#include "math/relaxation/SOR.h"
namespace Compute {

static void usingConjugateGradient(InLab11Outputs &outputs, InLab11Inputs &inputs) {
    MyBLAS::Matrix<MyBLAS::NumericType> A = inputs.input.coefficients;
    MyBLAS::Vector<MyBLAS::NumericType> b = inputs.input.constants;
    const size_t max_iterations = inputs.input.max_iterations;
    const MyBLAS::NumericType threshold = inputs.input.convergence_threshold;
    auto profiler = Profiler([&] {
        outputs.solution = MyRelaxationMethod::applyConjugateGradient(A, b, max_iterations, threshold);
    },100, 0, "Conjugate Gradient Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}

static void usingJacobiPreconditionedConjugateGradient(InLab11Outputs &outputs, InLab11Inputs &inputs) {
    auto profiler = Profiler([&inputs, &outputs] {
                        const MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
                        const MyBLAS::Vector<long double> &b = inputs.input.constants;
                        const size_t max_iterations = inputs.input.max_iterations;
                        const long double threshold = inputs.input.convergence_threshold;
                        outputs.solution = MyRelaxationMethod::applyJacobiPreconditionedConjugateGradient(A, b, max_iterations, threshold);
                    },100, 0, "Jacobi Preconditioned Conjugate Gradient Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}

} // namespace Compute
#endif // NE591_008_INLAB11_COMPUTE_H
