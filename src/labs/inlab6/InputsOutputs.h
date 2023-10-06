/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_INLAB6_INPUTOUTPUTS_H
#define NE591_008_INLAB6_INPUTOUTPUTS_H

#include <utility>

#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include "json.hpp"
#include "math/relaxation/RelaxationMethods.h"
#include "math/blas/MyBLAS.h"
#include "math/LinearSolver.h"

/**
 * @brief A structure to hold the input parameters for the relaxation method.
 *
 * This structure contains the convergence threshold, maximum number of iterations,
 * size of the matrices, coefficient matrix, vector of constants, and the set of methods.
 */
typedef struct Input {
    Input() = default;

    long double threshold = 0; ////< The convergence threshold
    size_t max_iterations = 0; ////< Maximum number of iterations to perform
    size_t n = 0; ///< Size of the matrices.
    MyBLAS::Matrix<long double> coefficients{}; ///< Coefficient matrix A
    MyBLAS::Vector<long double> constants{}; ///< Vector of constants b.

    std::set<MyRelaxationMethod::Type> methods = {};

    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the input parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["threshold"] = threshold;
        jsonMap["order"] = n;
        jsonMap["max-iterations"] = max_iterations;
        jsonMap["coefficients"] = coefficients.getData();
        jsonMap["constants"] = constants.getData();
        jsonMap["methods"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(methods.begin(), methods.end(), std::back_inserter(result), [](MyRelaxationMethod::Type method) {
                return MyRelaxationMethod::TypeKey(method);
            });
            return result;
        }();
    }
} InLab6Inputs;

/**
 * @brief A structure to hold the output of the relaxation method.
 *
 * This structure contains the input parameters, the solution, and the execution time.
 */
typedef struct Output {
    explicit Output(InLab6Inputs inputMatrices) {
        inputs = inputMatrices;
    };

    Output() = default;
    InLab6Inputs inputs;
    MyLinearSolvingMethod::Solution<long double> solution;
    long double execution_time = 0;

    /**
     * @brief Calculates the maximum residual of the solution.
     *
     * @return The maximum residual of the solution.
     */
    [[nodiscard]] long double getMaxResidual() const {
        const auto b_prime = inputs.coefficients * solution.x;
        return MyBLAS::max<long double>(MyBLAS::abs(inputs.constants - b_prime));
    }

    /**
     * @brief Converts the output parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the output parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["converged"] = solution.converged;

        jsonMap["iterations"]["maximum"] = inputs.max_iterations;
        jsonMap["iterations"]["actual"] = solution.iterations;

        jsonMap["iterative-error"]["maximum"] = inputs.threshold;
        jsonMap["iterative-error"]["actual"] = solution.iterative_error;

        jsonMap["solution"] = solution.x.getData();
        jsonMap["max-residual"] = getMaxResidual();

        jsonMap["execution-time-ns"] = execution_time;
    }
} InLab6Outputs;

#endif //NE591_008_INLAB6_INPUTOUTPUTS_H
