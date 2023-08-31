#include <iostream>
#include <iomanip>

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"
#include "utils/Helpers.h"
#include "CheckBounds.h"
#include "Compute.h"

void printInputs(boost::program_options::variables_map &vm) {
    const auto precision = vm["precision"].as<int>();
    std::cout<<std::setw(40)<<"Inputs\n";
    CommandLine::printLine();
    std::cout << "\tangle, x: "<<std::setprecision(precision) << vm["angle"].as<double_t>()<< "\n";
    std::cout << "\tconvergence-threshold, t: "<<std::setprecision(precision) << vm["convergence-threshold"].as<double_t>()<< "\n";
    std::cout << "\titerations, n: "<<std::setprecision(default_precision) << vm["iterations"].as<double_t>()<< "\n";
    CommandLine::printLine();
}

boost::program_options::options_description buildInputs() {
    boost::program_options::options_description arguments("Input variables");
    arguments.add_options()
            ("angle,x", boost::program_options::value<double_t>(), "= Angle in radians [abs(x) < 1.0]")
            ("convergence-threshold,t", boost::program_options::value<double_t>(), "= Iterative convergence threshold [e > 0]")
            ("iterations,n", boost::program_options::value<double_t>(), "= Total number of iterations");
    return arguments;
}

static void run(boost::program_options::variables_map &values) {

    const auto angle = values["angle"].as<double_t>();
    const auto convergence_threshold = values["convergence-threshold"].as<double_t>();
    const auto iterations = static_cast<size_t>(ceil(values["iterations"].as<double_t>()));

    const double_t my_sin_val = my_sin(angle, iterations, convergence_threshold);
    const double_t math_sin_val = sin(angle);
    const double_t truncation_error = abs(math_sin_val - my_sin_val);

    const auto precision = values["precision"].as<int>();
    std::cout<<std::setw(40)<<"Outputs\n";
    CommandLine::printLine();
    std::cout << "\tConverged at n="<<mySineVars.n<<" at convergence threshold: "<<mySineVars.current_threshold<<"\n\t...\n";
    std::cout << "\tmy_sin(x):"<<std::setw(37)<<std::setprecision(precision) << my_sin_val<< "\n";
    std::cout << "\tsin(x) from math.h:"<<std::setw(28)<<std::setprecision(precision) << math_sin_val<< "\n";
    std::cout << "\ttruncation error: "<<std::setw(30)<<std::setprecision(precision) << truncation_error << "\n";
    CommandLine::printLine();
}

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
