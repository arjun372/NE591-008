/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains the input and output definitions for this project.
 */

#pragma once

#include <utility>

#include "json.hpp"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

/**
 * @namespace MyBLAS
 * @brief Namespace for the Basic Linear Algebra Subprograms (BLAS) library.
 */
namespace MyBLAS {

/**
 * @struct Input
 * @brief Struct representing the input matrices for the BLAS library.
 */
typedef struct Input {
    Input() = default;

    size_t n = 0;                             ///< Size of the matrices.
    MyBLAS::Matrix<long double> coefficients; ///< LU matrix.
    MyBLAS::Vector<long double> constants;    ///< Vector of constants.

    /**
     * @brief Converts the input parameters to a JSON object.
     * @param jsonMap A reference to the JSON object to store the input parameters.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["n"] = n;
        jsonMap["coefficients"] = coefficients.getData();
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
    MyBLAS::Vector<long double> residual;

    /**
     * @brief Converts the output vector to a JSON object.
     * @param jsonMap A reference to the JSON object to store the output vector.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["solution"] = solution.getData();
        jsonMap["residual"] = residual.getData();
    }
} OutputVector;

} // namespace MyBLAS
