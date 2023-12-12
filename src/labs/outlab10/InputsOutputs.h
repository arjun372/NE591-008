/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB10_INPUTOUTPUTS_H
#define NE591_008_OUTLAB10_INPUTOUTPUTS_H

#include <utility>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include "json.hpp"

#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

#include "math/LinearSolver.h"
#include "math/blas/Constants.h"
#include "math/blas/Stats.h"

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
     * @brief The size of the input vector and matrix.
     */
    size_t n = 0;

    /**
     * @brief The maximum number of iterations to perform, 0 by default.
     */
    size_t max_iterations = 0;

    /**
     * @brief The stopping criterion, i.e. threshold, 1e-4 by default.
     */
    long double threshold = 1e-4;

    /**
     * @brief The constants matrix A
     */
    MyBLAS::Matrix<MyBLAS::NumericType> coefficients{};

    /**
     * @brief The coefficients vector b
     */
    MyBLAS::Vector<MyBLAS::NumericType> constants{};

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
        os << "n: " << input.n
           << "k: " << input.max_iterations
           << "e: " << input.threshold
           << ", outputJSON: " << input.outputJSON;
        return os;
    }
    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the input parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["n"] = n;
        jsonMap["stopping-criterion"] = threshold;
        jsonMap["max-iterations"] = max_iterations;
        jsonMap["coefficients"] = coefficients.getData();
        jsonMap["constants"] = constants.getData();
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
        ar & BOOST_SERIALIZATION_NVP(threshold);
        ar & BOOST_SERIALIZATION_NVP(max_iterations);
        ar & BOOST_SERIALIZATION_NVP(coefficients.getData());
        ar & BOOST_SERIALIZATION_NVP(constants.getData());
    }
} OutLab10Inputs;

/**
 * @brief A structure to hold the output
 *
 * This structure contains the input parameters, the solution, and the execution time.
 */
typedef struct Output {
    /**
     * @brief The computed values
     */

    MyLinearSolvingMethod::Solution<long double> solution;
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
        solution.toJSON(jsonMap["solution"]);
        //jsonMap["solution"] = solution;
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
        //ar & BOOST_SERIALIZATION_NVP(sum);
    }

    /**
     * @brief Overloaded stream insertion operator for the Output structure.
     *
     * This function allows the Output structure to be outputted to an ostream object in a formatted manner.
     *
     * @param os The ostream object where the Output structure will be outputted.
     * @param output The Output structure that will be outputted.
     * @return The ostream object with the Output structure outputted to it.
     */
    friend std::ostream& operator<<(std::ostream& os, const Output& output) {
        os << output.summary << std::endl << output.solution << std::endl;
        return os;
    }
} OutLab10Outputs;

#endif // NE591_008_OUTLAB10_INPUTOUTPUTS_H
