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
const char *ruleKey(IntegrationRule value) {
    static const char *IntegrationRuleKeys[] = {"none", "trapezoidal",
                                                "simpsons", "gaussian"};
    return IntegrationRuleKeys[static_cast<int>(value)];
}

/**
 * @brief Struct representing the input parameters for the Newton-Cotes
 * integration methods.
 */
typedef struct Input {
    long double a = 0;
    long double b = 0;
    size_t m = -1;
    bool flip_integral = false;
    std::set<IntegrationRule> integral_types = {RULE_NONE};
    void toJSON(nlohmann::json &jsonMap) {
        jsonMap["a"] = a;
        jsonMap["b"] = b;
        jsonMap["m"] = m;
        jsonMap["rules"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(integral_types.begin(), integral_types.end(),
                           std::back_inserter(result),
                           [](IntegrationRule rule) { return ruleKey(rule); });
            return result;
        }();
    }
} NewtonCotesInputs;

/**
 * @brief Struct representing the output parameters for the Newton-Cotes
 * integration methods.
 */
typedef struct Output {
    long double h;
    long double integral;
    void toJSON(nlohmann::json &jsonMap) {
        jsonMap["h"] = h;
        jsonMap["integral"] = integral;
    }
} NewtonCotesOutputs;

typedef std::map<std::string, std::string> Dictionary;
