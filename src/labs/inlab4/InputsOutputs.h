/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief This file contains the input and output definitions for this project.
 */

#pragma once

#include <utility>

#include "json.hpp"
#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"

/**
 * @struct Input
 * @brief Struct representing the input matrices for the BLAS library.
 */
typedef struct Input {
    Input() = default;

    size_t n = 0;                          ///< Size of the matrices.
    MyBLAS::Matrix<long double> LU;        ///< LU matrix.
    MyBLAS::Vector<long double> constants; ///< Vector of constants.

    /**
     * @brief Converts the input parameters to a JSON object.
     * @param jsonMap A reference to the JSON object to store the input parameters.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["n"] = n;
        jsonMap["LU"] = LU.getData();
        jsonMap["constants"] = constants.getData();
    }
} InputMatrices;

/**
 * @struct Output
 * @brief Struct representing the output vector for the BLAS library.
 */
typedef struct Output {
    Output() = default;
    MyBLAS::Vector<long double> solution; ///< Output vector.

    /**
     * @brief Converts the output vector to a JSON object.
     * @param jsonMap A reference to the JSON object to store the output vector.
     */
    void toJSON(nlohmann::json &jsonMap) const { jsonMap["solution"] = solution.getData(); }
} OutputVector;
