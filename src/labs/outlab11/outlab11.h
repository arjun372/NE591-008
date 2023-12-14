/**
 * @file outlab11.cpp
 * @author Arjun Earthperson
 * @date 10/27/2023
 * @brief This file contains the declaration for the OutLab11 class.
 *
 */

#ifndef NE591_008_OUTLAB11_H
#define NE591_008_OUTLAB11_H

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
 * @class OutLab11
 * @brief This class is a child of the MPI Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class OutLab11 : public Project<OutLab11Inputs, Parser, OutLab11Outputs> {

  public:
    /**
     * @brief This function is used to get the instance of the OutLab11 class.
     * @details This function follows the Singleton design pattern. It ensures that only one instance of the OutLab11
     * class is created.
     * @param args Command line arguments
     * @return Returns the instance of the OutLab11 class.
     */
    [[maybe_unused]] static OutLab11& getInstance(CommandLineArgs args) {
        static OutLab11 instance(args);
        return instance;
    }

    /**
     * @brief This function is used to delete the copy constructor.
     * @details This function ensures that the OutLab11 class cannot be copied. This is necessary because we are
     * following the Singleton design pattern.
     * @param OutLab11 const& The reference to the OutLab11 object to be copied.
     */
    OutLab11(OutLab11 const&) = delete;

    /**
     * @brief This function is used to delete the assignment operator.
     * @details This function ensures that the OutLab11 class cannot be assigned. This is necessary because we are
     * following the Singleton design pattern.
     * @param OutLab11 const& The reference to the OutLab11 object to be assigned.
     */
    void operator=(OutLab11 const&) = delete;

  protected:
    /**
     * @brief Constructor for the outlab11 class
     * @param args Command line arguments
     */
    explicit OutLab11(CommandLineArgs args) : Project(args) {}

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
            .ProjectName = "NE591: OutLab 11",
            .ProjectDescription = "Power Iterations Method",
            .SubmissionDate = "11/03/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = drawJuliaSet<__float128>(canvas, x, y, iterations),
        };
    }

    /**
     * @brief This function prints the results of the computation.
     * @param results The results of the computation.
     */
    static void printResults(OutLab11Outputs &results) {
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
    void run(OutLab11Outputs &outputs, OutLab11Inputs &inputs, boost::program_options::variables_map &values) override {

        nlohmann::json results;

        OutLab11Outputs powerIterationResults;
        Compute::usingPowerIteration(powerIterationResults, inputs);
        powerIterationResults.toJSON(results["outputs"]["power_iteration"]);
        Parser::printLine();

        inputs.toJSON(results["inputs"]);
        writeJSON(values["output-json"].as<std::string>(), results);
    }
};

#endif // NE591_008_OUTLAB11_H
