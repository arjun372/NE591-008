
#pragma once

#include <numeric>

#include "CommandLine.h"
#include "Project.h"

#include "Parser.h"
#include "Compute.h"
#include "Stopwatch.h"

typedef std::map<std::string, std::vector<std::string>> Dictionary;

class OutLab2 : public Project<LagrangePolynomialInputs, Parser, Dictionary> {

public:
    explicit OutLab2(CommandLineArgs args) : Project(args) {}

protected:

    HeaderInfo buildHeaderInfo() override {
        return HeaderInfo {
                .ProjectName = "OutLab 02",
                .ProjectDescription = "Lagrange Interpolating Polynomials - the fun parts",
                .SubmissionDate = "09/08/2023",
                .StudentName = "Arjun Earthperson",
                .HeaderArt = R"(
                       ,,                               .-.
                      || |                               ) )
                      || |   ,                          '-'
                      || |  | |
                      || '--' |
                ,,    || .----'
               || |   || |
               |  '---'| |
               '------.| |                                  _____
               ((_))  || |      (  _                       / /|\ \
               (o o)  || |      ))("),                    | | | | |
            ____\_/___||_|_____((__^_))____________________\_\|/_/__ldb)"
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

        const auto precision = getTerminal().getCurrentPrecision();
        // uniform interval over [a,b]
        const auto xMinMax = std::minmax_element(inputs.xData.begin(), inputs.xData.end());
        auto uniformXiInterval = std::vector<long double>(inputs.m, 0);
        fill_linspace(uniformXiInterval, *xMinMax.first, *xMinMax.second, inputs.m);
        outputs.emplace("x(i)", asStringVector(uniformXiInterval, precision));

        // Lagrange interpolated samples L[x(i)]
        auto lagrangePolynomials = std::vector<long double>(inputs.m, 0);
        fillLagrangePolys(lagrangePolynomials, uniformXiInterval, inputs.xData, inputs.fxData);
        outputs.emplace("L(x)", asStringVector(lagrangePolynomials, precision));

        // Interpolation error samples E[L(x)-f(x)]
        if(values.count("use-fx-function")) {
            // function f(x) evaluated at m points
            auto functionAtX = std::vector<long double>(inputs.m, 0);
            fill_fx(uniformXiInterval, functionAtX);
            outputs.emplace("f(x)", asStringVector(functionAtX, precision));

            auto interpolationErrors = std::vector<long double>(inputs.m, 0);
            fillInterpolationError(interpolationErrors, lagrangePolynomials, functionAtX);
            outputs.emplace("E(x)", asStringVector(interpolationErrors, precision));
        }


        writeCSV(values["output-csv"].as<std::string>(), outputs, {"i", "x(i)", "L(x)", "f(x)", "E(x)"});
    }

};
