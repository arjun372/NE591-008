/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB6_INPUTOUTPUTS_H
#define NE591_008_OUTLAB6_INPUTOUTPUTS_H

#include <utility>

#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include "math/blas/MyBLAS.h"
#include "math/LinearSolver.h"
#include "json.hpp"

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
        jsonMap["methods"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(methods.begin(), methods.end(), std::back_inserter(result), [](MyLinearSolvingMethod::Type method) {
                return MyLinearSolvingMethod::TypeKey(method);
            });
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
    explicit Output(OutLab6Inputs inputMatrices) {
        inputs = inputMatrices;
    };

    Output() = default;
    OutLab6Inputs inputs;
    MyLinearSolvingMethod::Solution<long double> solution;
    long double execution_time = 0;


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

        jsonMap["execution-time-ns"] = execution_time;
    }
} OutLab6Outputs;

#endif //NE591_008_OUTLAB6_INPUTOUTPUTS_H
