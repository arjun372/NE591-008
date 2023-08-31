/**
 * @file outlab1.cpp
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Entrypoint for outlab1
 */

#include <iostream>
#include <iomanip>

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"
#include "utils/Helpers.h"
#include "utils/CheckBounds.h"
#include "Compute.h"
#include "utils/Stopwatch.h"

/**
 * @brief This function builds the input options for the program.
 *
 * @return A description of the input options.
 *
 * It defines the options for angle, convergence threshold and iterations, and adds them to the options description.
 */
boost::program_options::options_description buildInputs() {
    boost::program_options::options_description arguments("Parameters");
    arguments.add_options()
            ("scalar,k", boost::program_options::value<double_t>(), "= Scalar multiplier, real number")
            ("num-rows,M", boost::program_options::value<double_t>(), "= Row length, natural number")
            ("num-cols,N", boost::program_options::value<double_t>(), "= Column length, natural number");
    return arguments;
}

/**
 * @brief Performs checks on the input parameters
 *
 * If the input value fails the check, the function will prompt the user to enter a new value.
 * The function will continue to prompt the user until a valid value is entered.
 *
 * @param values A map containing the input values to be checked.
 */
static void performInputChecks(boost::program_options::variables_map &values) {

    const auto max_errors = 10;
    auto errors = 0;
    while((errors++ < max_errors) && values["scalar"].empty()) {
        std::cout<<"Enter a value for the scalar multiplier (any real number): ";
        std::string input;
        std::cin >> input;
        replace(values, "scalar", std::stod(input));
    }

    while((errors++ < max_errors) && (values["num-rows"].empty() || failsNaturalNumberCheck(values["num-rows"].as<double_t>()))) {
        std::cout<<"Enter a value for the row count (any natural number): ";
        double_t input;
        std::cin >> input;
        replace(values, "num-rows", input);
    }

    while((errors++ < max_errors) && (values["num-rows"].empty() || failsNaturalNumberCheck(values["num-rows"].as<double_t>()))) {
        std::cout<<"Enter a value for the column count (any natural number): ";
        double_t input;
        std::cin >> input;
        replace(values, "num-cols", input);
    }

    if (errors == max_errors) {
        std::cerr<<"Too many input errors, exiting with status code 1.\n";
        exit(1);
    }
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
            .ProjectName = "OutLab 01",
            .ProjectDescription = "Non-vectorized, elementwise (mul, add) operations on 2D matrices",
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
