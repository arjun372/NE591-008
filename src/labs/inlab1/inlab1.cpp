/**
 * @file inlab1.cpp
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief Entrypoint for inlab1
 */

#include <iostream>
#include <iomanip>

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"
#include "utils/Stopwatch.h"

#include "Compute.h"
#include "ProcessInputs.h"

/**
 * @brief This function runs the main logic of the program.
 *
 * @param values A map of variable names to their values.
 *
 * It retrieves the angle, convergence threshold and iterations from the variable map, and then computes the sine
 * of the angle using both the standard library function and a custom implementation. It also measures the time taken
 * for each computation and the truncation error between the two results.
 */
static void run(boost::program_options::variables_map &values) {

    const auto angle = values["angle"].as<long double>();
    const auto convergence_threshold = values["convergence-threshold"].as<long double>();
    const auto iterations = static_cast<size_t>(ceil(values["iterations"].as<long double>()));

    std::cout << std::setw(40) << "Profile\n";
    CommandLine::printLine();
    Stopwatch<Microseconds> clock;
    // my_sin(x)
    clock.restart();
    const long double my_sin_val = my_sin(angle, iterations, convergence_threshold);
    clock.click();
    std::cout << " my_sin(x) completed in: " << static_cast<long double>(clock.duration().count()) << "ns" << std::endl;

    // sin(x)
    clock.restart();
    const long double math_sin_val = sin(angle);
    clock.click();
    std::cout << " sin(x) completed in: " << static_cast<long double>(clock.duration().count()) << "ns" << std::endl;

    // truncation error
    const long double truncation_error = abs(math_sin_val - my_sin_val);
    CommandLine::printLine();

    const auto precision = values["precision"].as<int>();
    std::cout<<std::setw(40)<<"Outputs\n";
    CommandLine::printLine();
    std::cout << "\tConverged at n="<<mySineVars.n<<" at convergence threshold: "<<mySineVars.current_threshold<<"\n\t...\n";
    std::cout << "\tmy_sin(x):"<<std::setw(37)<<std::setprecision(precision) << my_sin_val<< "\n";
    std::cout << "\tsin(x) from math.h:"<<std::setw(28)<<std::setprecision(precision) << math_sin_val<< "\n";
    std::cout << "\ttruncation error: "<<std::setw(30)<<std::setprecision(precision) << truncation_error << "\n";
    CommandLine::printLine();
}

/**
 * @brief The main function of the program.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @return The exit status of the program.
 *
 * It sets up the program information, builds the input options, parses the command line arguments, checks the input
 * values, prints the input values, runs the main logic of the program, and then returns the exit status.
 */
int main(int argc, char **argv) {


    HeaderInfo programInfo {
            .ProjectName = "InLab 01",
            .ProjectDescription = "Iterative Taylor series approximation of sin(x)",
            .SubmissionDate = "08/30/2023",
            .StudentName = "Arjun Earthperson",
    };

    auto inputs = buildInputs();
    auto values = CommandLine(programInfo, inputs, argc, argv).getArguments();
    performInputChecks(values);
    printInputs(values);
    run(values);
    return 0;
}
