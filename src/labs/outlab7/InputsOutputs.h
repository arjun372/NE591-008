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

    size_t n = 1;
    std::string outputJSON;

    friend std::ostream& operator<<(std::ostream& os, const Input& input) {
        os << "n: " << input.n<< ", outputJSON: " << input.outputJSON;
        return os;
    }

    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the input parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["n"] = n;
    }

    // TODO:: DOCUMENT
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(n);
    }
} OutLab7Inputs;

/**
 * @brief A structure to hold the output of the relaxation method.
 *
 * This structure contains the input parameters, the solution, and the execution time.
 */
typedef struct Output {
    MyBLAS::NumericType sum = 0;
    MyBLAS::Stats::Summary<long double> summary;

    /**
     * @brief Converts the output parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the output parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["total_sum"] = sum;
        summary.toJSON(jsonMap["benchmark"]);
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(sum);
    }
} OutLab7Outputs;

#endif // NE591_008_OUTLAB7_INPUTOUTPUTS_H
