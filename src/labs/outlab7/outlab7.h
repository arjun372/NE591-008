/**
 * @file outlab7.cpp
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the declaration for the OutLab7 class.
 *
 */

#ifndef NE591_008_OUTLAB7_H
#define NE591_008_OUTLAB7_H

#include <boost/program_options.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <variant>
#include <mpi.h>

#include "InputsOutputs.h"
#include "Parser.h"

#include "CommandLine.h"
#include "Project.h"

#include "Compute.h"
#include "json.hpp"

/**
 * @class OutLab7
 * @brief This class is a child of the Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class OutLab7 : public Project<OutLab7Inputs, Parser, OutLab7Outputs> {

  public:
    /**
     * @brief Constructor for the outlab7 class
     * @param args Command line arguments
     */
    explicit OutLab7(CommandLineArgs args) : Project(args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        auto x = 0.13;
        auto y = -0.66;
        auto iterations = 80;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        canvas.tone_map.growth_rate = 0.25;
        printJuliaSet<__float128>(canvas, x, y, iterations); //"o█■"
        std::cout << "Julia set at (" << x << "," << y << "), " << iterations << " iterations\n";
        return {
            .ProjectName = "NE591: OutLab 07",
            .ProjectDescription = "",
            .SubmissionDate = "10/13/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = " ",
        };
    }

    /**
     * @brief This function runs the computation.
     * @param outputs The outputs of the computation.
     * @param inputs The inputs to the computation.
     * @param values The values used in the computation.
     */
    void run(OutLab7Outputs &outputs, OutLab7Inputs &inputs, boost::program_options::variables_map &values) override {

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);

        Parser::printLine();
        outputs.toJSON(results["outputs"]);
        writeJSON(values["output-json"].as<std::string>(), results);
    }
};

#endif // NE591_008_OUTLAB7_H
