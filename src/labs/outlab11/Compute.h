/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the declaration and implementation of the Compute namespace.
 */

#ifndef NE591_008_OUTLAB11_COMPUTE_H
#define NE591_008_OUTLAB11_COMPUTE_H

#include "math/relaxation/PowerIteration.h"

/**
 * @namespace Compute
 * @brief This namespace provides a function to compute the sum of a series.
 */
namespace Compute {

static void usingPowerIteration(OutLab11Outputs &outputs, OutLab11Inputs &inputs) {
    MyBLAS::Matrix<MyBLAS::NumericType> A = inputs.input.coefficients;
    const size_t max_iterations = inputs.input.max_iterations;
    const MyBLAS::NumericType threshold = inputs.input.threshold;
    auto profiler = Profiler([&] {
                        outputs.solution = MyRelaxationMethod::applyPowerIteration2<MyBLAS::Matrix, MyBLAS::Vector, MyBLAS::NumericType>(A, max_iterations, threshold);
    },100, 0, "Power Iterations Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}

} // namespace Compute
#endif // NE591_008_OUTLAB11_COMPUTE_H
