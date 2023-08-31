/*
 * Arjun Earthperson
 * Bounds checking methods for inlab1 in NE591-008.
*/

#pragma once

#include <iostream>
#include <boost/program_options.hpp>

#include "utils/CommandLine.h"

static bool failsCheck1(double_t value) {
    const double_t max = 1.0f;
    if (abs(value) >= max) {
        std::cerr <<"Error: " << "abs(x) is greater than " << std::setprecision (19) << max << "\n";
        return true;
    }
    return false;
}

static bool failsCheck2(double_t value) {
    const double_t min = 0.0f;
    if (value <= min) {
        std::cerr <<"Error: " << "error threshold is not greater than " << std::setprecision (19) << min << "\n";
        return true;
    }
    return false;
}

static bool failsCheck3(double_t value) {
    const double_t min = 0;
    auto error = false;
    if (value <= min) {
        std::cerr <<"Error: " << "number of iterations should be a positive number\n";
        error = true;
    }
    if (ceil(value) != floor(value)) {
        std::cerr <<"Error: " << "number of iterations should be a natural number\n";
        error = true;
    }

    if (value == 0) {
        std::cerr <<"Error: " << "number of iterations cannot be zero\n";
        error = true;
    }

    return error;
}


static void performInputChecks(boost::program_options::variables_map &values) {

    while(values["angle"].empty() || failsCheck1(values["angle"].as<double_t>())) {
        std::cout<<"Enter a value for the angle x [radian]: ";
        double_t input;
        std::cin >> input;
        replace(values, "angle", input);
    }

    while(values["convergence-threshold"].empty() || failsCheck2(values["convergence-threshold"].as<double_t>())) {
        std::cout<<"Enter a value for the stopping criterion e [e > 0]: ";
        double_t input;
        std::cin >> input;
        replace(values, "convergence-threshold", input);
    }

    while(values["iterations"].empty() || failsCheck3(values["iterations"].as<double_t>())) {
        std::cout<<"Enter maximum number of iterations (natural number): ";
        double_t input;
        std::cin >> input;
        replace(values, "iterations", input);
    }
}
