/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB_5_INPUTOUTPUTS_H
#define NE591_008_OUTLAB_5_INPUTOUTPUTS_H

#include <utility>

#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include "json.hpp"
#include "math/relaxation_methods/RelaxationMethods.h"
#include "math/blas/MyBLAS.h"

// TODO:: DOCUMENT
typedef struct Input {
    Input() = default;

//    // Copy constructor
//    Input(const Input& other)
//            : threshold(other.threshold),
//              max_iterations(other.max_iterations),
//              n(other.n),
//              coefficients(other.coefficients),
//              constants(other.constants),
//              methods(other.methods) {}

    long double threshold = 0; ////< The convergence threshold
    size_t max_iterations = 0; ////< Maximum number of iterations to perform
    size_t n = 0; ///< Size of the matrices.
    MyBLAS::Matrix<long double> coefficients{}; ///< Coefficient matrix A
    MyBLAS::Vector<long double> constants{}; ///< Vector of constants b.

    std::set<MyRelaxationMethod::Type> methods = {};

    // TODO:: DOCUMENT
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
} InputMatrices;

// TODO:: DOCUMENT
typedef struct Output {
    explicit Output(InputMatrices inputMatrices) {
        inputs = inputMatrices;
    };

    Output() = default;
    InputMatrices inputs;
    MyRelaxationMethod::Solution<long double> solution;
    long double execution_time = 0;

    // TODO:: DOCUMENT
    [[nodiscard]] long double getMaxResidual() const {
        const auto b_prime = inputs.coefficients * solution.x;
        return MyBLAS::max<long double>(MyBLAS::abs(inputs.constants - b_prime));
    }

    // TODO:: DOCUMENT
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
} RelaxationMethodOutputs;

#endif //NE591_008_OUTLAB_5_INPUTOUTPUTS_H
