/**
 * @file inlab2.cpp
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Declaration for the InLab2 class
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


typedef std::map<std::string, std::vector<std::string>> Dictionary;

class InLab2 : public Project<LagrangePolynomialInputs, Parser, Dictionary> {

public:
    explicit InLab2(CommandLineArgs args) : Project(args) {}

protected:

    HeaderInfo buildHeaderInfo() override {
        return {
                .ProjectName = "InLab 02",
                .ProjectDescription = "I/O Setup for Lagrange Interpolating Polynomials",
                .SubmissionDate = "09/01/2023",
                .StudentName = "Arjun Earthperson",
                .HeaderArt = R"(
         @\_______/@
        @|XXXXXXXX |
       @ |X||    X |
      @  |X||    X |
     @   |XXXXXXXX |
    @    |X||    X |             V
   @     |X||   .X |
  @      |X||.  .X |                      V
 @      |%XXXXXXXX%||
@       |X||  . . X||
        |X||   .. X||                               @     @
        |X||  .   X||.                              ||====%
        |X|| .    X|| .                             ||    %
        |X||.     X||   .                           ||====%
       |XXXXXXXXXXXX||     .                        ||    %
       |XXXXXXXXXXXX||         .                 .  ||====% .
       |XX|        X||                .        .    ||    %  .
       |XX|        X||                   .          ||====%   .
       |XX|        X||              .          .    ||    %     .
       |XX|======= X||============================+ || .. %  ........
===== /            X||                              ||    %
                   X||           /)                 ||    %
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
)",
        };
    }

    /**
     * @brief This function is responsible for the main logic of the program.
     *
     * @param values A map of variables parsed from the command line arguments.
     *
     * The run() function creates an InputParams object and populates it with the appropriate values from the command line
     * arguments.
     *
     * After that, it creates an output map and populates it with the indices, uniform interval, Lagrange interpolated
     * samples, and interpolation error samples.
     *
     * If the "use-fx-function" command line argument is present, it also calculates the interpolation errors and adds them
     * to the output map.
     *
     * Finally, it writes the output map to a CSV file specified by the "output-csv" command line argument.
     */
    void run(Dictionary &outputs, Input &inputs, boost::program_options::variables_map &values) override {

        // create indices 1 -> m
        std::vector<size_t> indexVector(inputs.m);
        std::iota(indexVector.begin(), indexVector.end(), 1);
        outputs.emplace("i", asStringVector(indexVector));

        // uniform interval over [a,b]
        const auto xMinMax = std::minmax_element(inputs.xData.begin(), inputs.xData.end());
        auto uniformXiInterval = std::vector<long double>(inputs.m, 0);
        fill_linspace(uniformXiInterval, *xMinMax.first, *xMinMax.second, inputs.m);
        outputs.emplace("x(i)", asStringVector(uniformXiInterval));

        // Lagrange interpolated samples L[x(i)]
        auto lagrangePolynomials = std::vector<long double>(inputs.m, 0);
        fillLagrangePolys(lagrangePolynomials, uniformXiInterval, inputs.xData, inputs.fxData);
        outputs.emplace("L(x)", asStringVector(lagrangePolynomials));

        // Interpolation error samples E[L(x)-f(x)]
        if(values.count("use-fx-function")) {
            auto functionAtX = std::vector<long double>(inputs.m, 0);
            fillInterpolationError(functionAtX, lagrangePolynomials, inputs.fxData);
            outputs.emplace("f(x)", asStringVector(lagrangePolynomials));

            auto interpolationErrors = std::vector<long double>(inputs.m, 0);
            fillInterpolationError(interpolationErrors, lagrangePolynomials, inputs.fxData);
            outputs.emplace("E(x)", asStringVector(lagrangePolynomials));
        }


        writeCSV(values["output-csv"].as<std::string>(), outputs, {"i", "x(i)", "L(x)", "f(x)", "E(x)"});
    }

};
