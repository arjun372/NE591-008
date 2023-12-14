/**
 * @file outlab12.cpp
 * @author Arjun Earthperson
 * @date 10/27/2023
 * @brief This file contains the declaration for the OutLab12 class.
 *
 */

#ifndef NE591_008_OUTLAB12_H
#define NE591_008_OUTLAB12_H

#include <boost/program_options.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <variant>

#include "InputsOutputs.h"
#include "Parser.h"

#include "CommandLine.h"
#include "Project.h"

#include "Compute.h"
#include "json.hpp"
#include "math/relaxation/RelaxationMethods.h"

/**
 * @class OutLab12
 * @brief This class is a child of the MPI Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class OutLab12 : public Project<OutLab12Inputs, Parser, OutLab12Outputs> {

  public:
    /**
     * @brief This function is used to get the instance of the OutLab12 class.
     * @details This function follows the Singleton design pattern. It ensures that only one instance of the OutLab12
     * class is created.
     * @param args Command line arguments
     * @return Returns the instance of the OutLab12 class.
     */
    [[maybe_unused]] static OutLab12& getInstance(CommandLineArgs args) {
        static OutLab12 instance(args);
        return instance;
    }

    /**
     * @brief This function is used to delete the copy constructor.
     * @details This function ensures that the OutLab12 class cannot be copied. This is necessary because we are
     * following the Singleton design pattern.
     * @param OutLab12 const& The reference to the OutLab12 object to be copied.
     */
    OutLab12(OutLab12 const&) = delete;

    /**
     * @brief This function is used to delete the assignment operator.
     * @details This function ensures that the OutLab12 class cannot be assigned. This is necessary because we are
     * following the Singleton design pattern.
     * @param OutLab12 const& The reference to the OutLab12 object to be assigned.
     */
    void operator=(OutLab12 const&) = delete;

  protected:
    /**
     * @brief Constructor for the outlab12 class
     * @param args Command line arguments
     */
    explicit OutLab12(CommandLineArgs args) : Project(args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        const __float128 x = 0.13;
        const __float128 y = -0.66;
        const size_t iterations = 80;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        canvas.tone_map.growth_rate = 0.25;
        return {
            .ProjectName = "NE591: OutLab 12",
            .ProjectDescription = "Inverse Power Iteration Method",
            .SubmissionDate = "11/10/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = drawJuliaSet<__float128>(canvas, x, y, iterations),
        };
    }

    /**
     * @brief This function prints the results of the computation.
     * @param results The results of the computation.
     */
    static void printResults(OutLab12Outputs &results) {
        std::cout << "\ttotal iterations          : " << (results.solution.iterations) << std::endl;
        std::cout << "\tconverged                 : " << (results.solution.converged ? "Yes" : "No") << std::endl;
        std::cout << "\titerative error           : " << (results.solution.iterative_error) << std::endl;}

    /**
     * @brief This function is used to perform the main computation.
     * @details The computation is divided among the processes. Each process computes a partial sum and then the partial
     * sums are combined using a binary tree communication logic.
     * @param outputs The outputs of the computation.
     * @param inputs The inputs to the computation.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return Returns false to indicate that the computation was successful.
     */
    void run(OutLab12Outputs &outputs, OutLab12Inputs &inputs, boost::program_options::variables_map &values) override {

        nlohmann::json results;

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_DIRECT_POWER_ITERATION)) {
            OutLab12Outputs dPowerIterationResults;
            Compute::usingDirectPowerIteration(dPowerIterationResults, inputs);
            dPowerIterationResults.toJSON(results["outputs"]["direct-power-iteration"]);
            Parser::printLine();
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_RAYLEIGH_QUOTIENT_POWER_ITERATION)) {
            OutLab12Outputs rPowerIterationResults;
            Compute::usingRayleighQuotientPowerIteration(rPowerIterationResults, inputs);
            rPowerIterationResults.toJSON(results["outputs"]["rayleigh-quotient-power-iteration"]);
            Parser::printLine();
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_INVERSE_POWER_ITERATION)) {
            OutLab12Outputs iPowerIterationResults;
            Compute::usingRayleighQuotientPowerIteration(iPowerIterationResults, inputs);
            iPowerIterationResults.toJSON(results["outputs"]["inverse-power-iteration"]);
            Parser::printLine();
        }


        inputs.toJSON(results["inputs"]);
        writeJSON(values["output-json"].as<std::string>(), results);
    }
};

#endif // NE591_008_OUTLAB12_H
