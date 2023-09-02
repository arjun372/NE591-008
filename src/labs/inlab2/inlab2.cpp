/**
 * @file inlab2.cpp
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Entrypoint containing the main() and run() methods.
 *
 */

#include <variant>
#include <numeric>

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"
#include "Compute.h"
#include "ProcessInputs.h"

static void run(boost::program_options::variables_map &values) {

    std::cout<<"run!\n"<<values.size();
    InputParams inputs = {
            .n = static_cast<size_t>(values["num-points"].as<long double>()),
            .m = static_cast<size_t>(values["num-samples"].as<long double>()),
            .xData = values["x-points"].as<std::vector<long double>>(),
            .fxData = values["fx-points"].as<std::vector<long double>>(),
    };

    //build an output map
    std::map<std::string, std::vector<std::string>> outputs;

    // create indices 1 -> m
    std::vector<size_t> indexVector(inputs.m);
    std::iota(indexVector.begin(), indexVector.end(), 1);
    outputs.emplace("idx", asStringVector(indexVector));

    // uniform interval over [a,b]
    const auto xMinMax = std::minmax_element(inputs.xData.begin(), inputs.xData.end());
    auto uniformXiInterval = std::vector<long double>(inputs.m, 0);
    fill_linspace(uniformXiInterval, *xMinMax.first, *xMinMax.second, inputs.m);
    outputs.emplace("x(i)", asStringVector(uniformXiInterval));

    // Lagrange interpolated samples L[x(i)]
    auto lagrangePolynomials = std::vector<long double>(inputs.m, 0);
    fillLagrangePolys(lagrangePolynomials, uniformXiInterval, inputs.xData, inputs.fxData);
    outputs.emplace("L[x(i)]", asStringVector(lagrangePolynomials));

    // Interpolation error samples E[L(x)-f(x)]
    if(values.count("use-fx-function")) {
        auto interpolationErrors = std::vector<long double>(inputs.m, 0);
        fillInterpolationError(interpolationErrors, lagrangePolynomials, inputs.fxData);
        outputs.emplace("E[L(x)-f(x)]", asStringVector(lagrangePolynomials));
    }


    writeCSV(values["output-csv"].as<std::string>(), outputs, {"idx", "x(i)", "L[x(i)]", "E[L(x)-f(x)]"});
}


/**
 * @brief The main function of the program.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @return The exit status of the program.
 *
 * The main function first sets up the program information and builds the input options. It then creates a CommandLine
 * object and uses it to parse the command line arguments into a map of variables. After that, it checks the input
 * values and prints them. Finally, it runs the main logic of the program and returns the exit status.
 */
int main(int argc, char **argv) {


    HeaderInfo programInfo{
            .ProjectName = "InLab 02",
            .ProjectDescription = "I/O Setup for Lagrange Interpolation Polynomials",
            .SubmissionDate = "09/01/2023",
            .StudentName = "Arjun Earthperson",
    };

    auto inputs = buildInputs();
    auto values = CommandLine(programInfo, inputs, argc, argv).getArguments();
    performInputChecks(values);
    printInputs(values);
    run(values);
    return 0;
}
