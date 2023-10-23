/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB7_INPUTOUTPUTS_H
#define NE591_008_OUTLAB7_INPUTOUTPUTS_H

#include <utility>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include "json.hpp"
#include "math/blas/Constants.h"

// TODO:: DOCUMENT
typedef struct Input {
    Input() = default;

    size_t p = 1;
    size_t n = 1;

    friend std::ostream& operator<<(std::ostream& os, const Input& input) {
        os << "p: " << input.p << ", n: " << input.n;
        return os;
    }

    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the input parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["n"] = p;
        jsonMap["p"] = n;
    }

    // TODO:: DOCUMENT
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(p);
        ar & BOOST_SERIALIZATION_NVP(n);
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
