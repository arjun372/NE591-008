/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief Compute methods for project milestone 1 in NE591-008.
 */

#ifndef NE591_008_PROJECT2_COMPUTE_H
#define NE591_008_PROJECT2_COMPUTE_H

#include <cmath>
#include <iostream>

#include "math/blas/matrix/Matrix.h"
#include "math/factorization/LU.h"
#include "math/factorization/LUP.h"
#include "math/relaxation/PointJacobi.h"

/**
 * @namespace Compute
 * @brief Namespace containing methods for solving linear systems using various methods.
 */
namespace Compute {

/**
 * @brief Solves a linear system using LUP decomposition.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingLUP(SolverOutputs &outputs, SolverInputs &inputs) {

    // auto A = inputs.solverParams.
    // MyBLAS::Vector<long double> &b = inputs.input.constants;

//    if (!MyFactorizationMethod::passesPreChecks(A, b)) {
//        std::cerr << "Aborting LUP calculation\n";
//        return;
//    }

    {
        const size_t maxRuns = 10;
        auto runTimes = std::vector<long double>(maxRuns);
        Stopwatch<Nanoseconds> timer;
        for (outputs.runs = 0; outputs.runs < maxRuns; outputs.runs++) {
            timer.restart();
            {
                //outputs.solution = MyBLAS::LUP::applyLUP(A, b);
            }
            timer.click();
            runTimes[outputs.runs] = (timer.duration().count());
        }
        auto stats = computeMeanStd(runTimes);
        outputs.mean_execution_time = boost::accumulators::mean(stats);
        outputs.stddev_execution_time = std::sqrt(boost::accumulators::variance(stats));
        outputs.runs = maxRuns;
    }
    outputs.solution.converged = true;
}

/**
 * @brief Solves a linear system using the Point Jacobi method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingPointJacobi(SolverOutputs &outputs, SolverInputs &inputs) {
//    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
//    MyBLAS::Vector<long double> &b = inputs.input.constants;
//    const size_t max_iterations = inputs.solverParams.max_iterations;
//    const long double threshold = inputs.solverParams.threshold;

//    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
//        std::cerr << "Aborting Point jacobi calculation\n";
//        return;
//    }

    {
        const size_t maxRuns = 10;
        auto runTimes = std::vector<long double>(maxRuns);
        Stopwatch<Nanoseconds> timer;
        for (outputs.runs = 0; outputs.runs < maxRuns; outputs.runs++) {
            timer.restart();
            {
                //outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold);
            }
            timer.click();
            runTimes[outputs.runs] = (timer.duration().count());
        }
        auto stats = computeMeanStd(runTimes);
        outputs.mean_execution_time = boost::accumulators::mean(stats);
        outputs.stddev_execution_time = std::sqrt(boost::accumulators::variance(stats));
        outputs.runs = maxRuns;
    }
}

/**
 * @brief Solves a linear system using the Gauss-Seidel method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingGaussSeidel(SolverOutputs &outputs, SolverInputs &inputs) {
//    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
//    MyBLAS::Vector<long double> &b = inputs.input.constants;
//    const size_t max_iterations = inputs.solverParams.max_iterations;
//    const long double threshold = inputs.solverParams.threshold;

//    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
//        std::cerr << "Aborting Gauss Seidel calculation\n";
//        return;
//    }

    {
        const size_t maxRuns = 10;
        auto runTimes = std::vector<long double>(maxRuns);
        Stopwatch<Nanoseconds> timer;
        for (outputs.runs = 0; outputs.runs < maxRuns; outputs.runs++) {
            timer.restart();
            {
               // outputs.solution = MyRelaxationMethod::applyGaussSeidel<long double>(A, b, max_iterations, threshold);
            }
            timer.click();
            runTimes[outputs.runs] = (timer.duration().count());
        }
        auto stats = computeMeanStd(runTimes);
        outputs.mean_execution_time = boost::accumulators::mean(stats);
        outputs.stddev_execution_time = std::sqrt(boost::accumulators::variance(stats));
        outputs.runs = maxRuns;
    }
}

/**
 * @brief Solves a linear system using the Successive Over-Relaxation (SOR) method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingSOR(SolverOutputs &outputs, SolverInputs &inputs) {
//    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
//    MyBLAS::Vector<long double> &b = inputs.input.constants;
//    const size_t max_iterations = inputs.input.max_iterations;
//    const long double threshold = inputs.input.threshold;
//    const long double omega = inputs.input.relaxation_factor;

//    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
//        std::cerr << "Aborting SOR calculation\n";
//        return;
//    }

    {
        const size_t maxRuns = 10;
        auto runTimes = std::vector<long double>(maxRuns);
        Stopwatch<Nanoseconds> timer;
        for (outputs.runs = 0; outputs.runs < maxRuns; outputs.runs++) {
            timer.restart();
            {
//                outputs.solution = MyRelaxationMethod::applySOR<long double>(A, b, max_iterations, threshold, omega);
            }
            timer.click();
            runTimes[outputs.runs] = (timer.duration().count());
        }
        auto stats = computeMeanStd(runTimes);
        outputs.mean_execution_time = boost::accumulators::mean(stats);
        outputs.stddev_execution_time = std::sqrt(boost::accumulators::variance(stats));
        outputs.runs = maxRuns;
    }
}

/**
 * @brief Solves a linear system using the Jacobi SOR method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingJacobiSOR(SolverOutputs &outputs, SolverInputs &inputs) {
//    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
//    MyBLAS::Vector<long double> &b = inputs.input.constants;
//    const size_t max_iterations = inputs.input.max_iterations;
//    const long double threshold = inputs.input.threshold;
//    const long double omega = inputs.input.relaxation_factor;

//    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
//        std::cerr << "Aborting SOR point jacobi calculation\n";
//        return;
//    }

    {
        const size_t maxRuns = 10;
        auto runTimes = std::vector<long double>(maxRuns);
        Stopwatch<Nanoseconds> timer;
        for (outputs.runs = 0; outputs.runs < maxRuns; outputs.runs++) {
            timer.restart();
            {
//                outputs.solution = MyRelaxationMethod::applyPointJacobi<long double>(A, b, max_iterations, threshold, omega);
            }
            timer.click();
            runTimes[outputs.runs] = (timer.duration().count());
        }
        auto stats = computeMeanStd(runTimes);
        outputs.mean_execution_time = boost::accumulators::mean(stats);
        outputs.stddev_execution_time = std::sqrt(boost::accumulators::variance(stats));
        outputs.runs = maxRuns;
    }
}

/**
 * @brief Solves a linear system using the Symmetric SOR method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingSymmetricSOR(SolverOutputs &outputs, SolverInputs &inputs) {
//    MyBLAS::Matrix<long double> &A = inputs.input.coefficients;
//    MyBLAS::Vector<long double> &b = inputs.input.constants;
//    const size_t max_iterations = inputs.input.max_iterations;
//    const long double threshold = inputs.input.threshold;
//    const long double omega = inputs.input.relaxation_factor;

//    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
//        std::cerr << "Aborting symmetric SOR calculation\n";
//        return;
//    }

    {
        const size_t maxRuns = 10;
        auto runTimes = std::vector<long double>(maxRuns);
        Stopwatch<Nanoseconds> timer;
        for (outputs.runs = 0; outputs.runs < maxRuns; outputs.runs++) {
            timer.restart();
            {
//                outputs.solution = MyRelaxationMethod::applySSOR<long double>(A, b, max_iterations, threshold, omega);
            }
            timer.click();
            runTimes[outputs.runs] = (timer.duration().count());
        }
        auto stats = computeMeanStd(runTimes);
        outputs.mean_execution_time = boost::accumulators::mean(stats);
        outputs.stddev_execution_time = std::sqrt(boost::accumulators::variance(stats));
        outputs.runs = maxRuns;
    }
}
} // namespace Compute

#endif // NE591_008_PROJECT2_COMPUTE_H
