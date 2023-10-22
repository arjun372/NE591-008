/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB7_INPUTOUTPUTS_H
#define NE591_008_OUTLAB7_INPUTOUTPUTS_H

#include <utility>

#include "json.hpp"
#include "math/blas/Constants.h"

// TODO:: DOCUMENT
typedef struct Input {
    Input() = default;

    size_t p = 1;
    size_t n = 1;

    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the input parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["n"] = p;
        jsonMap["p"] = n;
    }
} OutLab7Inputs;

/**
 * @brief A structure to hold the output of the relaxation method.
 *
 * This structure contains the input parameters, the solution, and the execution time.
 */
typedef struct Output {
    explicit Output(OutLab7Inputs inputMatrices) { inputs = inputMatrices; };

    Output() = default;
    OutLab7Inputs inputs;
    MyBLAS::NumericType sum = 0;

    /**
     * @brief Converts the output parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the output parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["sum"] = sum;
    }
} OutLab7Outputs;

#endif // NE591_008_OUTLAB7_INPUTOUTPUTS_H
