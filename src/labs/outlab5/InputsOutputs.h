/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB_5_INPUTOUTPUTS_H
#define NE591_008_OUTLAB_5_INPUTOUTPUTS_H

#include <utility>

#include "json.hpp"
#include "math/blas/BLAS.h"
#include "math/blas/Stats.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

/**
 * @struct Input
 * @brief Struct representing the input matrices for the BLAS library.
 */
typedef struct Input {
    Input() = default;

    size_t n = 0;                               ///< Size of the matrices.
    MyBLAS::Matrix<long double> coefficients{}; ///< LU matrix.
    MyBLAS::Vector<long double> constants{};    ///< Vector of constants.

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

typedef struct Map {
    Map() = default;

    MyBLAS::Matrix<long double> L;
    MyBLAS::Matrix<long double> U;
    MyBLAS::Matrix<long double> P;

    /**
     * @brief Converts the input parameters to a JSON object.
     * @param jsonMap A reference to the JSON object to store the input parameters.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["lower"] = L.getData();
        jsonMap["upper"] = U.getData();
        jsonMap["permutation"] = P.getData();
    }
} IntermediateResults;

/**
 * @struct Output
 * @brief Struct representing the output vector for the BLAS library.
 */
typedef struct Output {
    Output() = default;

    MyBLAS::Vector<long double> solution; ///< Output vector.
    MyBLAS::Vector<long double> residual;
    long double max_residual = 0;

    /**
     * @brief Converts the output vector to a JSON object.
     * @param jsonMap A reference to the JSON object to store the output vector.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["solution"] = solution.getData();
        jsonMap["residual"] = residual.getData();
        jsonMap["max-residual"] = MyBLAS::Stats::max<long double>(MyBLAS::Stats::abs(residual));
    }
} Results;

#endif // NE591_008_OUTLAB_5_INPUTOUTPUTS_H
