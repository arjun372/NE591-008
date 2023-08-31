/*
 * Arjun Earthperson
 * Compute methods for inlab1 in NE591-008.
*/

#pragma once

#include <iostream>
#include <cmath>

#include "utils/CommandLine.h"

typedef struct {
    size_t n;
    double_t value;
} Factorial;

typedef struct {
    double_t running_sum;
    Factorial computed_factorial;
    double_t previous_factorial;
    double_t current_threshold;
    double_t target_threshold;
    double_t x;
    size_t n;
    size_t N;
} TaylorSeriesVariables;

static TaylorSeriesVariables mySineVars{};

void printVars(const TaylorSeriesVariables &variables) {
    std::cout<<"running sum: "<<std::setprecision(max_precision)<<variables.running_sum<<"\n";
    std::cout<<"prev factorial: "<<std::setprecision(max_precision)<<variables.previous_factorial<<"\n";
    std::cout<<"n: "<<std::setprecision(max_precision)<<variables.n<<"\n";
    std::cout<<"current threshold: "<<std::setprecision(max_precision)<<variables.current_threshold<<"\n";
    CommandLine::printLine();
}

double_t naive_factorial(const size_t N) {
    double_t accumulator = 1.0f;
    for(auto n = N; n >= 1; n--) {
        accumulator *= static_cast<double_t>(n);
    }
    return accumulator;
}

double_t my_sisd_sin(TaylorSeriesVariables &vars) {
    // terminate since we are over the iteration limit
    if (vars.n > vars.N) {
        return vars.running_sum;
    }
    // terminate since we have converged
    if (vars.current_threshold <= vars.target_threshold) {
        return vars.running_sum;
    }

    // n<=N && (ce > e)
    const double_t direction = ((vars.n % 2) == 0) ? 1.0f : -1.0f; // positive if n is even, negative if n is odd.
    const auto two_n_plus_1 = static_cast<double_t>(2 * vars.n + 1); // (2n+1)
    const double_t x_power_two_n_plus_1 = pow(vars.x, two_n_plus_1); // x^(2n+1)
    const double_t current_factorial = naive_factorial(static_cast<size_t>(two_n_plus_1));// (2n+1) * prev_factorial
    const double_t current_threshold = x_power_two_n_plus_1 / current_factorial;
    const double_t accumulated = vars.running_sum + direction * current_threshold;

    vars.current_threshold = current_threshold;
    vars.running_sum = accumulated;
    (vars.n)++;
    return my_sisd_sin(vars);
}

double_t my_naive_sin(const double_t x, double_t prev_threshold, double_t target_threshold, double_t sum, size_t n, size_t N) {

    // terminate since we are over the iteration limit
    if (n > N) {
        return sum;
    }
    // terminate since we have converged
    if (prev_threshold <= target_threshold) {
        return sum;
    }

    // n<=N && (ce > e)
    const double_t direction = ((n % 2) == 0) ? 1.0f : -1.0f; // positive if n is even, negative if n is odd.
    const auto two_n_plus_1 = static_cast<double_t>(2 * n + 1); // (2n+1)
    const double_t x_power_two_n_plus_1 = pow(x, two_n_plus_1); // x^(2n+1)
    const double_t current_factorial = naive_factorial(static_cast<size_t>(two_n_plus_1));// (2n+1) * prev_factorial
    const double_t current_threshold = x_power_two_n_plus_1 / current_factorial;
    const double_t accumulated = sum + direction * current_threshold;
    return my_naive_sin(x, current_threshold, target_threshold, accumulated, ++n, N);
}

double_t my_sin(const double_t angle, const size_t iterations, const double_t convergence_threshold) {
    mySineVars.running_sum = 0.0f;
    mySineVars.previous_factorial = 1.0f;
    mySineVars.current_threshold = INFINITY;
    mySineVars.target_threshold = convergence_threshold;
    mySineVars.x = angle;
    mySineVars.n = 0;
    mySineVars.N = iterations;
    return my_sisd_sin(mySineVars);
//    return my_naive_sin(mySineVars.x, mySineVars.current_threshold, mySineVars.target_threshold, mySineVars.running_sum, mySineVars.n, mySineVars.N);
}
