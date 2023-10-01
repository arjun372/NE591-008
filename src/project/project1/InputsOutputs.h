/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_PROJECT1_INPUTOUTPUTS_H
#define NE591_008_PROJECT1_INPUTOUTPUTS_H

#include <utility>

#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include "json.hpp"
#include "math/blas/MyBLAS.h"

/**
 * @brief Enumeration representing the different integration rules.
 */
enum SolverMethod {
    METHOD_LUP,
    METHOD_POINT_JACOBI,
    METHOD_GAUSS_SEIDEL,
    METHOD_SOR
};

/**
 * @brief Function to get the string representation of an IntegrationRule.
 * @param value The IntegrationRule value.
 * @return The string representation of the IntegrationRule.
 */
const char* methodKey(SolverMethod value) {
    static const char* SolverMethodKeys[] = {
            "LUP",
            "point-jacobi",
            "gauss-seidel",
            "SOR"
    };
    return SolverMethodKeys[static_cast<int>(value)];
}

/**
    * @struct Input
    * @brief Struct representing the input matrices for the BLAS library.
    */
typedef struct Input {
    Input() = default;
    std::set<SolverMethod> solver_methods = { METHOD_LUP };
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
            std::transform(solver_methods.begin(), solver_methods.end(), std::back_inserter(result), [](SolverMethod method) {
                return methodKey(method);
            });
            return result;
        }();
    }
} SolverInputs;

typedef struct Map {
    Map() = default;

    MyBLAS::Matrix<long double> L;
    MyBLAS::Matrix<long double> U;
    MyBLAS::Matrix<long double> P;
    MyBLAS::Matrix<long double> x;

    MyBLAS::Matrix<long double> diffusion_matrix_A; ///< Diffusion Matrix A
    MyBLAS::Vector<long double> right_hand_side_vector_B; ///< Right Hand Side Vector B

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
    MyBLAS::Matrix<long double> fluxes;

    /**
     * @brief Converts the output vector to a JSON object.
     * @param jsonMap A reference to the JSON object to store the output vector.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["solution"] = solution.getData();
        jsonMap["residual"] = residual.getData();
        jsonMap["max-residual"] = MyBLAS::max<long double>(MyBLAS::abs(residual));
    }
} SolverOutputs;

#endif //NE591_008_PROJECT1_INPUTOUTPUTS_H
