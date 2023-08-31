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
#include "utils/Helpers.h"
#include "CheckBounds.h"
#include "Compute.h"
#include "utils/Stopwatch.h"

/**
 * @brief This function prints the input values.
 *
 * @param vm A map of variable names to their values.
 *
 * It retrieves the precision, angle, convergence threshold and iterations from the variable map and prints them.
 */
void printInputs(boost::program_options::variables_map &vm) {
    const auto precision = vm["precision"].as<int>();
    std::cout<<std::setw(40)<<"Inputs\n";
    CommandLine::printLine();
    std::cout << "\tangle, x: "<<std::setprecision(precision) << vm["angle"].as<double_t>()<< "\n";
    std::cout << "\tconvergence-threshold, t: "<<std::setprecision(precision) << vm["convergence-threshold"].as<double_t>()<< "\n";
    std::cout << "\titerations, n: "<<std::setprecision(default_precision) << vm["iterations"].as<double_t>()<< "\n";
    CommandLine::printLine();
}

/**
 * @brief This function builds the input options for the program.
 *
 * @return A description of the input options.
 *
 * It defines the options for angle, convergence threshold and iterations, and adds them to the options description.
 */
boost::program_options::options_description buildInputs() {
    boost::program_options::options_description arguments("Input variables");
    arguments.add_options()
            ("angle,x", boost::program_options::value<double_t>(), "= Angle in radians [abs(x) < 1.0]")
            ("convergence-threshold,t", boost::program_options::value<double_t>(), "= Iterative convergence threshold [e > 0]")
            ("iterations,n", boost::program_options::value<double_t>(), "= Total number of iterations");
    return arguments;
}

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

    const auto angle = values["angle"].as<double_t>();
    const auto convergence_threshold = values["convergence-threshold"].as<double_t>();
    const auto iterations = static_cast<size_t>(ceil(values["iterations"].as<double_t>()));

    std::cout << std::setw(40) << "Profile\n";
    CommandLine::printLine();
    Stopwatch<Microseconds> clock;
    // my_sin(x)
    clock.restart();
    const double_t my_sin_val = my_sin(angle, iterations, convergence_threshold);
    clock.click();
    std::cout << " my_sin(x) completed in: " << static_cast<double_t>(clock.duration().count()) << "ns" << std::endl;

    // sin(x)
    clock.restart();
    const double_t math_sin_val = sin(angle);
    clock.click();
    std::cout << " sin(x) completed in: " << static_cast<double_t>(clock.duration().count()) << "ns" << std::endl;

    // truncation error
    const double_t truncation_error = abs(math_sin_val - my_sin_val);
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
