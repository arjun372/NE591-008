/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 09/08/2023
 * @brief This file contains the input and output definitions for this project.
 * user inputs.
 */

#pragma once

/**
 * @brief Enumeration representing the different integration rules.
 */
enum IntegrationRule {
    RULE_NONE,
    RULE_COMPOSITE_TRAPEZOIDAL,
    RULE_COMPOSITE_SIMPSONS,
    RULE_GAUSSIAN_QUADRATURE,
};

/**
 * @brief Function to get the string representation of an IntegrationRule.
 * @param value The IntegrationRule value.
 * @return The string representation of the IntegrationRule.
 */
const char* ruleKey(IntegrationRule value) {
    static const char* IntegrationRuleKeys[] = {
            "none",
            "trapezoidal",
            "simpsons",
            "gaussian"
    };
    return IntegrationRuleKeys[static_cast<int>(value)];
}

/**
 * @brief Struct representing the input parameters for the Newton-Cotes integration methods.
 */
typedef struct Input {
    long double a = 0; ///< The lower limit of integration.
    long double b = 0; ///< The upper limit of integration.
    size_t m = -1; ///< The number of sub-intervals to divide the integration interval into.
    size_t n = -1; ///< The number of quadrature points if Gauss-Legendre Quadrature will be used.
    bool flip_integral = false; ///< If true, the integral will be evaluated in reverse order (from b to a).
    std::set<IntegrationRule> integral_types = { RULE_NONE }; ///< A set of integration rules to be applied.
    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap A reference to the JSON object to store the input parameters.
     */
    void toJSON(nlohmann::json &jsonMap) {
        jsonMap["a"] = a;
        jsonMap["b"] = b;
        jsonMap["m"] = m;
        jsonMap["rules"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(integral_types.begin(), integral_types.end(), std::back_inserter(result), [](IntegrationRule rule) {
                return ruleKey(rule);
            });
            return result;
        }();

        if (integral_types.count(RULE_GAUSSIAN_QUADRATURE)) {
            jsonMap["n"] = n;
        }
    }
} NewtonCotesInputs;

/**
 * @brief Struct representing the output parameters for the Newton-Cotes integration methods.
 */
typedef struct Output {
    long double h;
    long double integral;
    void toJSON(nlohmann::json &jsonMap) {
        jsonMap["h"] = h;
        jsonMap["integral"] = integral;
    }
} NewtonCotesOutputs;

/**
 * @struct Outputs
 * @brief A structure to store the results of the Gauss-Legendre quadrature.
 *
 * This structure contains the integral value, quadrature points, and weights
 * obtained from the Gauss-Legendre quadrature method. It also provides a
 * method to convert the data into a JSON format.
 */
typedef struct Outputs {
    long double integral;
    std::vector<long double> quadrature_points = {};
    std::vector<long double> weights = {};
    /**
     * @brief Converts the data in the structure to a JSON format.
     *
     * This method takes a reference to a JSON object and populates it with
     * the data from the structure. The keys in the JSON object are "weights",
     * "quadrature-points", and "integral".
     *
     * @param jsonMap A reference to a JSON object to be populated with the data.
     */
    void toJSON(nlohmann::json &jsonMap) {
        jsonMap["integral"] = integral;
        jsonMap["weights"] = [this]() -> std::map<std::string, long double> {
            std::map<std::string, long double> result;
            std::transform(weights.begin(), weights.end(), std::inserter(result, result.end()),
                           [i = 0](const long double& value) mutable {
                               return std::make_pair("w" + std::to_string(i++), value);
                           });
            return result;
        }();
        jsonMap["quadrature-points"] = [this]() -> std::map<std::string, long double> {
            std::map<std::string, long double> result;
            std::transform(quadrature_points.begin(), quadrature_points.end(), std::inserter(result, result.end()),
                          [i = 0](const long double& value) mutable {
                              return std::make_pair("n" + std::to_string(i++), value);
                          });
            return result;
        }();
    }
} GaussLegendreOutputs;

/**
 * @typedef Dictionary
 * @brief A convenience typedef for a map of string key-value pairs.
 *
 * This typedef is used to represent a dictionary-like data structure, where
 * both the keys and values are strings.
 */
typedef std::map<std::string, std::string> Dictionary;
