/**
 * @file InputsOutputs.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the input and output definitions for this project.
 */

#ifndef NE591_008_OUTLAB12_INPUTOUTPUTS_H
#define NE591_008_OUTLAB12_INPUTOUTPUTS_H

#include <utility>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include "json.hpp"

#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"

#include "blas/solver/LinearSolver.h"
#include "math/blas/Constants.h"
#include "solver/LinearSolverParams.h"

/**
 * @brief A structure to hold the input parameters for the relaxation method.
 *
 * This structure contains a size_t variable 'n' and a string 'outputJSON'.
 */
typedef struct Input {
    std::set<MyBLAS::Solver::Type> methods = {};
    MyBLAS::Solver::Parameters<long double> input;
    MyBLAS::Vector<long double> known_solution;

    /**
     * @brief Converts the input parameters to a JSON object.
     *
     * @param jsonMap The JSON object to which the input parameters are added.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["threshold"] = input.convergence_threshold;
        jsonMap["order"] = input.n;
        jsonMap["max-iterations"] = input.max_iterations;
        jsonMap["coefficients"] = input.coefficients.getData();
        jsonMap["constants"] = input.constants.getData();
        jsonMap["known-solution"] = known_solution.getData();
        jsonMap["methods"] = [this]() -> std::vector<std::string> {
            std::vector<std::string> result;
            std::transform(methods.begin(), methods.end(), std::back_inserter(result),
                           [](MyBLAS::Solver::Type method) { return MyBLAS::Solver::TypeKey(method); });
            return result;
        }();
    }
} OutLab12Inputs;

/**
 * @brief A structure to hold the output
 *
 * This structure contains the input parameters, the solution, and the execution time.
 */
typedef struct Output {
    /**
     * @brief The computed values
     */

    MyBLAS::Solver::Solution<long double> solution;
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
} OutLab12Outputs;

#endif // NE591_008_OUTLAB12_INPUTOUTPUTS_H
