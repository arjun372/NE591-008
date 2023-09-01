/**
 * @file inlab2.cpp
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Entrypoint containing the main() and run() methods.
 *
 */

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"
#include "Compute.h"
#include "ProcessInputs.h"

static void run(boost::program_options::variables_map &values) {

    std::cout<<"run!\n"<<values.size();
//    InputParams inputs = {
//            .k = values["scalar"].as<long double>(),
//            .M = static_cast<size_t>(values["m-rank"].as<long double>()),
//            .N = static_cast<size_t>(values["n-rank"].as<long double>()),
//            .J = static_cast<size_t>(values["j-rank"].as<long double>()),
//    };

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
            .ProjectDescription = "Lagrange Interpolation Polynomials",
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
