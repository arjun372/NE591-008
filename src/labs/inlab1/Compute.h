/*
 * Arjun Earthperson
 * Compute methods for InLab01 in NE591-008.
*/

#ifndef NE591_008_COMPUTE_H
#define NE591_008_COMPUTE_H

#include <math.h>

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

double_t my_sisd_sin(TaylorSeriesVariables &variables) {
    printVars(variables);
    // terminate since we are over the iteration limit
    if (variables.n > variables.N) {
        return variables.running_sum;
    }
    // terminate since we have converged
    if (variables.current_threshold <= variables.target_threshold) {
        return variables.running_sum;
    }

    // n<=N && (ce > e)
    const double_t pow = powf64(-1.0f, variables.n); // (-1)^n
    const double_t two_n_plus_1 = (2.0f * variables.n + 1.0f); // (2n+1)
    const double_t numerator = powf64(variables.x, two_n_plus_1);
    variables.previous_factorial *= two_n_plus_1; // (2n+1) * prev_factorial
    variables.current_threshold = numerator / variables.previous_factorial;
    variables.running_sum += pow * variables.current_threshold; // accumulate
    variables.n++;
    return my_sisd_sin(variables);
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
//    double_t current_threshold = x_power_two_n_plus_1 / current_factorial;
//    current_threshold = ((n % 2) == 0) ? current_threshold : -current_threshold; // positive if n is even, negative if n is odd.
//    const double_t accumulated = sum + current_threshold;
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
    return my_naive_sin(mySineVars.x, mySineVars.current_threshold, mySineVars.target_threshold, mySineVars.running_sum, mySineVars.n, mySineVars.N);
}


#endif //NE591_008_COMPUTE_H
