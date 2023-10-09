/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB6_INPUTOUTPUTS_H
#define NE591_008_OUTLAB6_INPUTOUTPUTS_H

#include <utility>

#include "json.hpp"
#include "math/LinearSolver.h"
#include "math/blas/Matrix.h"
#include "math/blas/MyBLAS.h"
#include "math/blas/Ops.h"
#include "math/blas/Vector.h"

/**
 * @brief A structure to hold the input parameters for the relaxation method.
 *
 * This structure contains the convergence threshold, maximum number of iterations,
 * size of the matrices, coefficient matrix, vector of constants, and the set of methods.
 */
typedef struct Input {
    Input() = default;

    std::set<MyLinearSolvingMethod::Type> methods = {};
    MyLinearSolvingMethod::Input<long double> input;
    MyBLAS::Vector<long double> known_solution;

    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the input parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["threshold"] = input.threshold;
        jsonMap["order"] = input.n;
        jsonMap["max-iterations"] = input.max_iterations;
        jsonMap["coefficients"] = input.coefficients.getData();
        jsonMap["constants"] = input.constants.getData();
        jsonMap["known-solution"] = known_solution.getData();
        jsonMap["relaxation-factor"] = input.relaxation_factor;
        jsonMap["methods"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(methods.begin(), methods.end(), std::back_inserter(result),
                           [](MyLinearSolvingMethod::Type method) { return MyLinearSolvingMethod::TypeKey(method); });
            return result;
        }();
    }
} OutLab6Inputs;

/**
 * @brief A structure to hold the output of the relaxation method.
 *
 * This structure contains the input parameters, the solution, and the execution time.
 */
typedef struct Output {
    explicit Output(OutLab6Inputs inputMatrices) { inputs = inputMatrices; };

    Output() = default;
    OutLab6Inputs inputs;
    MyLinearSolvingMethod::Solution<long double> solution;

    long double mean_execution_time = std::numeric_limits<long double>::quiet_NaN();
    long double stddev_execution_time = std::numeric_limits<long double>::quiet_NaN();
    size_t runs = 0;

    /**
     * @brief Calculates and returns the error of the solution.
     *
     * This function calculates the error of the solution by comparing it with the known solution.
     * The error is calculated using the L2 Norm (Euclidean Norm) and then taking the square root of the result.
     *
     * @return The error of the solution as a long double.
     */
    [[maybe_unused]] [[nodiscard]] long double getSolutionError() const {
        return std::sqrt(MyBLAS::L2Norm(solution.x, inputs.known_solution));
    }

    /**
     * @brief Converts the output parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the output parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["converged"] = solution.converged;

        jsonMap["iterations"]["maximum"] = inputs.input.max_iterations;
        jsonMap["iterations"]["actual"] = solution.iterations;

        jsonMap["iterative-error"]["maximum"] = inputs.input.threshold;
        jsonMap["iterative-error"]["actual"] = solution.iterative_error;

        jsonMap["solution"] = solution.x.getData();
        jsonMap["max-residual"] = solution.getMaxResidual(inputs.input.coefficients, inputs.input.constants);
        jsonMap["l2-error"] = getSolutionError();
        jsonMap["wall-time-ns"]["mean"] = mean_execution_time;
        jsonMap["wall-time-ns"]["std"] = stddev_execution_time;
        jsonMap["wall-time-ns"]["samples"] = 10;
    }
} OutLab6Outputs;

#endif // NE591_008_OUTLAB6_INPUTOUTPUTS_H
