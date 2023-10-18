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
#include "math/blas/BLAS.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/Stats.h"
#include "math/blas/vector/Vector.h"
#include "physics/diffusion/DiffusionMatrix.h"
#include "physics/diffusion/DiffusionParams.h"

/**
 * @struct Input
 * @brief Struct representing the input matrices for the BLAS library.
 */
typedef struct Input {
    Input() = default;
    std::set<MyLinearSolvingMethod::Type> methods = {};

    MyLinearSolvingMethod::Parameters<long double> solverParams;

    MyBLAS::Matrix<long double> sources = MyBLAS::Matrix<long double>();

    MyPhysics::Diffusion::Params<long double> diffusionParams;
    MyPhysics::Diffusion::Matrix<long double> diffusionCoefficients;
    MyBLAS::Vector<long double> diffusionConstants;

    std::string fluxOutputDirectory;

    /**
     * @brief Converts the input parameters to a JSON object.
     * @param jsonMap A reference to the JSON object to store the input parameters.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["D"] = diffusionParams.getDiffusionCoefficient();
        jsonMap["cross-section"] = diffusionParams.getMacroscopicRemovalCrossSection();
        jsonMap["dimensions"]["a"] = diffusionParams.getA();
        jsonMap["dimensions"]["b"] = diffusionParams.getB();
        jsonMap["mesh"]["m"] = diffusionParams.getM();
        jsonMap["mesh"]["n"] = diffusionParams.getN();
        jsonMap["mesh"]["𝛿"] = diffusionParams.getDelta();
        jsonMap["mesh"]["𝛾"] = diffusionParams.getGamma();
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
    explicit Output(SolverInputs inputMatrices) { inputs = std::move(inputMatrices); };
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

        jsonMap["iterations"]["maximum"] = inputs.solverParams.max_iterations;
        jsonMap["iterations"]["actual"] = solution.iterations;

        jsonMap["iterative-error"]["maximum"] = inputs.solverParams.threshold;
        jsonMap["iterative-error"]["actual"] = solution.iterative_error;

        jsonMap["solution"] = solution.x.getData();
        jsonMap["residual"] = residual.getData();
        jsonMap["max-residual"] = MyBLAS::Stats::max<long double>(MyBLAS::Stats::abs(residual));

        // if LUP is used
        // jsonMap["l2-error"] = getSolutionError();

        jsonMap["wall-time-ns"]["mean"] = mean_execution_time;
        jsonMap["wall-time-ns"]["std"] = stddev_execution_time;
        jsonMap["wall-time-ns"]["samples"] = 10;
    }
} SolverOutputs;

#endif // NE591_008_PROJECT2_INPUTOUTPUTS_H
