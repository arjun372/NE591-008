/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the declaration and implementation of the Compute namespace.
 */

#ifndef NE591_008_OUTLAB12_COMPUTE_H
#define NE591_008_OUTLAB12_COMPUTE_H

#include "math/relaxation/PowerIteration.h"

/**
 * @namespace Compute
 * @brief This namespace provides a function to compute the sum of a series.
 */
namespace Compute {

static void usingDirectPowerIteration(OutLab12Outputs &outputs, OutLab12Inputs &inputs) {
    inputs.input.initial_guess = Random::generate_vector<MyBLAS::NumericType>(inputs.input.n, -1, 1);
    std::cout << inputs.input << std::endl;
    auto profiler = Profiler([&] {
                        outputs.solution = MyRelaxationMethod::applyDirectPowerIteration<MyBLAS::Matrix, MyBLAS::Vector, MyBLAS::NumericType>(inputs.input);
    },100, 0, "Direct Power Iteration Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}

static void usingRayleighQuotientPowerIteration(OutLab12Outputs &outputs, OutLab12Inputs &inputs) {
    inputs.input.initial_guess = Random::generate_vector<MyBLAS::NumericType>(inputs.input.n, -1, 1);
    std::cout << inputs.input << std::endl;
    auto profiler = Profiler([&] {
                        outputs.solution = MyRelaxationMethod::applyRayleighQuotientPowerIteration<MyBLAS::Matrix, MyBLAS::Vector, MyBLAS::NumericType>(inputs.input);
    }, 100, 0, "Rayleigh Quotient Power Iteration Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}

static void usingInversePowerIteration(OutLab12Outputs &outputs, OutLab12Inputs &inputs) {
    inputs.input.initial_guess = Random::generate_vector<MyBLAS::NumericType>(inputs.input.n, -1, 1);
    std::cout << inputs.input << std::endl;
    auto profiler = Profiler([&] {
                        outputs.solution = MyRelaxationMethod::applyInversePowerIteration<MyBLAS::Matrix, MyBLAS::Vector, MyBLAS::NumericType>(inputs.input);
    }, 100, 0, "Inverse Power Iteration Method").run();
    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;
}



} // namespace Compute
#endif // NE591_008_OUTLAB12_COMPUTE_H
