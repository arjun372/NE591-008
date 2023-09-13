/**
 * @file outlab3.cpp
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief Declaration for the OutLab3 class
 *
 */

#pragma once

#include <variant>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>

#include "utils/Helpers.h"
#include "utils/CheckBounds.h"
#include "utils/FileParser.h"
#include "utils/Project.h"
#include "extern/function.h"

#include "Parser.h"
#include "Compute.h"
#include "InputsOutputs.h"
#include "utils/json.hpp"

/**
 * @class OutLab3
 * @brief A class for performing numerical integration using Composite Newton-Cotes Formulas
 *
 * This class is derived from the Project class and implements the numerical integration
 * using Composite Newton-Cotes Formulas. It takes command line arguments, parses the inputs,
 * performs the integration, and outputs the results.
 */
class OutLab3 : public Project<NewtonCotesInputs, Parser, Dictionary> {

public:
    /**
     * @brief Constructor for the InLab3 class
     * @param args Command line arguments
     */
    explicit OutLab3(CommandLineArgs args) : Project(args) {}

protected:
    /**
     * @brief Builds the header information for the project
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        return {
                .ProjectName = "InLab 03",
                .ProjectDescription = "Numerical Integration using Composite Newton-Cotes Formulas",
                .SubmissionDate = "09/08/2023",
                .StudentName = "Arjun Earthperson",
        };
    }

    /**
     * @brief Runs the numerical integration using Composite Newton-Cotes Formulas
     * @param outputs Dictionary object to store the outputs
     * @param inputs NewtonCotesInputs object containing the parsed inputs
     * @param values Boost program_options variables_map containing command line arguments
     */
    void run(Dictionary &outputs, NewtonCotesInputs &inputs, boost::program_options::variables_map &values) override {

        // declare the JSON object and file
        nlohmann::json JSONFile;

        // write the inputs to file
        nlohmann::json inputsJSON;
        inputs.toJSON(inputsJSON);
        JSONFile["inputs"] = inputsJSON;

        nlohmann::json outputsJSON;
        std::vector<NewtonCotesOutputs> computeOutputs;


        const auto precision = values["precision"].as<int>();
        std::cout<<std::setw(40)<<"Outputs\n";
        Parser::printLine();

        if (inputs.integral_types.count(RULE_COMPOSITE_TRAPEZOIDAL)) {
            NewtonCotesOutputs output1;
            fillUsingTrapezoidal(output1, inputs);
            computeOutputs.push_back(output1);
            nlohmann::json json1;
            output1.toJSON(json1);
            outputsJSON[ruleKey(RULE_COMPOSITE_TRAPEZOIDAL)] = json1;
            std::cout << "Integral over [a,b] using trapezoidal rule: "<<std::setprecision(precision)<<output1.integral<<std::endl;
            Parser::printLine();
        }

        if (inputs.integral_types.count(RULE_COMPOSITE_SIMPSONS)) {
            NewtonCotesOutputs output2;
            fillUsingSimpsons(output2, inputs);
            computeOutputs.push_back(output2);
            nlohmann::json json2;
            output2.toJSON(json2);
            outputsJSON[ruleKey(RULE_COMPOSITE_SIMPSONS)] = json2;
            std::cout << "Integral over [a,b] using Simpson's rule:   "<<std::setprecision(precision)<<output2.integral<<std::endl;
            Parser::printLine();
        }

        if (inputs.integral_types.count(RULE_GAUSSIAN_QUADRATURE)) {
            NewtonCotesOutputs output3;
            fillUsingGaussianQuadratures(output3, inputs);
            computeOutputs.push_back(output3);
            //nlohmann::json json3;
            //output3.toJSON(json3);
            //outputsJSON[ruleKey(RULE_GAUSSIAN_QUADRATURE)] = json3;
            Parser::printLine();
        }

        // create the outputs object
        JSONFile["outputs"] = outputsJSON;
        writeJSON(values["output-json"].as<std::string>(), JSONFile);
    }

};
