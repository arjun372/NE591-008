/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_PROJECT2_INPUTOUTPUTS_H
#define NE591_008_PROJECT2_INPUTOUTPUTS_H

#include <utility>

#include "json.hpp"
#include "math/LinearSolver.h"
#include "math/blas/Matrix.h"
#include "math/blas/MyBLAS.h"
#include "math/blas/Vector.h"

/**
 * @struct Input
 * @brief Struct representing the input matrices for the BLAS library.
 */
typedef struct Input {
    Input() = default;
    std::set<MyLinearSolvingMethod::Type> methods = {};

    // NOTE: in input, store the constants and coefficient matrices only if the LUP method is used
    MyLinearSolvingMethod::Input<long double> input;
//    MyBLAS::Matrix<long double> diffusion_matrix_A;       ///< Diffusion Matrix A
//    MyBLAS::Vector<long double> right_hand_side_vector_B; ///< Right Hand Side Vector B*/

    long double a = 1;
    long double b = 1;
    size_t m = 1;
    size_t n = 1;
    long double delta = 1;
    long double gamma = 1;
    long double diffusion_coefficient = 0.0f;
    long double macroscopic_removal_cross_section = 0.0f;
    MyBLAS::Matrix<long double> sources = MyBLAS::Matrix<long double>();

    /**
     * @brief Converts the input parameters to a JSON object.
     * @param jsonMap A reference to the JSON object to store the input parameters.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["D"] = diffusion_coefficient;
        jsonMap["cross-section"] = macroscopic_removal_cross_section;
        jsonMap["dimensions"]["a"] = a;
        jsonMap["dimensions"]["b"] = b;
        jsonMap["mesh"]["m"] = m;
        jsonMap["mesh"]["n"] = n;
        jsonMap["mesh"]["𝛿"] = delta;
        jsonMap["mesh"]["𝛾"] = gamma;
        jsonMap["methods"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(methods.begin(), methods.end(), std::back_inserter(result),
                           [](MyLinearSolvingMethod::Type method) { return MyLinearSolvingMethod::TypeKey(method); });
            return result;
        }();
    }
} SolverInputs;

/**
 * @struct Output
 * @brief Struct representing the output vector for the BLAS library.
 */
typedef struct Output {

    Output() = default;
    explicit Output(SolverInputs inputMatrices) { inputs = inputMatrices; };
    SolverInputs inputs;
    MyLinearSolvingMethod::Solution<long double> solution;

    long double mean_execution_time = std::numeric_limits<long double>::quiet_NaN();
    long double stddev_execution_time = std::numeric_limits<long double>::quiet_NaN();
    size_t runs = 0;

    MyBLAS::Vector<long double> residual;
    MyBLAS::Matrix<long double> fluxes;

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
        jsonMap["residual"] = residual.getData();
        jsonMap["max-residual"] = MyBLAS::max<long double>(MyBLAS::abs(residual));

        // if LUP is used
        // jsonMap["l2-error"] = getSolutionError();

        jsonMap["wall-time-ns"]["mean"] = mean_execution_time;
        jsonMap["wall-time-ns"]["std"] = stddev_execution_time;
        jsonMap["wall-time-ns"]["samples"] = 10;
    }
} SolverOutputs;

#endif // NE591_008_PROJECT2_INPUTOUTPUTS_H
