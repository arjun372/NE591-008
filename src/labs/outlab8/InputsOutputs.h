/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_INLAB8_INPUTOUTPUTS_H
#define NE591_008_INLAB8_INPUTOUTPUTS_H

#include <utility>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include "json.hpp"
#include "math/blas/Constants.h"

/**
 * @brief A structure to hold the input parameters for the relaxation method.
 *
 * This structure contains a size_t variable 'n' and a string 'outputJSON'.
 */
typedef struct Input {
    /**
     * @brief The default constructor for the Input struct
     */
    Input() = default;
    /**
     * @brief The number of iterations for the relaxation method.
     *
     * This variable is initialized to 1 by default.
     */
    size_t n = 1;
    /**
     * @brief The output file name in JSON format.
     *
     * This string variable holds the name of the output file where the results will be stored in JSON format.
     */
    std::string outputJSON;
    /**
     * @brief Overloaded stream insertion operator for the Input structure.
     *
     * This function allows the Input structure to be outputted to an ostream object in a formatted manner.
     *
     * @param os The ostream object where the Input structure will be outputted.
     * @param input The Input structure that will be outputted.
     * @return The ostream object with the Input structure outputted to it.
     */
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
    /**
     * @brief Function to serialize the Input structure.
     *
     * This function uses the Boost library to serialize the Input structure, allowing it to be saved to a file or
     * sent over a network.
     *
     * @param ar The archive where the serialized data will be stored.
     * @param version The version of the serialization library.
     */
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(n);
    }
} OutLab7Inputs;

/**
 * @brief A structure to hold the output
 *
 * This structure contains the input parameters, the solution, and the execution time.
 */
typedef struct Output {
    /**
     * @brief The total sum of the given series
     *
     * This variable holds the total sum of the given series.
     */
    MyBLAS::NumericType sum = 0;
    /**
     * @brief The summary of the benchmark runs
     * This structure holds the summary statistics of computation runtime, including the mean, standard deviation, etc.
     */
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
    /**
     * @brief Function to serialize the Output structure.
     *
     * This function uses the Boost library to serialize the Output structure, allowing it to be saved to a file or
     * sent over a network.
     *
     * @param ar The archive where the serialized data will be stored.
     * @param version The version of the serialization library.
     */
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(sum);
    }
} OutLab7Outputs;

#endif // NE591_008_INLAB8_INPUTOUTPUTS_H
