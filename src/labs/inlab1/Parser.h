#pragma once

#include "CommandLine.h"
#include "CheckBounds.h"
#include "Helpers.h"

typedef struct Input {
    long double x; ///< The x value.
    long double target_threshold; ///< The target threshold.
    size_t N; ///< The maximum number of iterations.
} TaylorSeriesInputs;

class Parser : public CommandLine<TaylorSeriesInputs> {

public:
    explicit Parser(const HeaderInfo &headerInfo, const CommandLineArgs &args) : CommandLine(headerInfo, args) {

    }

    explicit Parser() = default;

protected:
/**
     * @brief This function builds the input options for the program.
     *
     * @return A description of the input options.
     *
     * It defines the options for angle, convergence threshold and iterations, and adds them to the options description.
     */
    void buildInputArguments(boost::program_options::options_description &values) override {
        values.add_options()
                ("angle,x", boost::program_options::value<long double>(), "= angle in radians [abs(x) < 1.0]")
                ("convergence-threshold,t", boost::program_options::value<long double>(), "= iterative convergence threshold [e > 0]")
                ("iterations,n", boost::program_options::value<long double>(), "= total number of iterations");
    }

    /**
     * @brief Performs input checks on the input variables
     *
     * If the input value fails the check, the function will prompt the user to enter a new value.
     * The function will continue to prompt the user until a valid value is entered.
     *
     * @param values A map containing the input values to be checked.
     */
    void performInputArgumentsCheck(boost::program_options::variables_map &values) override {

        std::string input;
        while(values["angle"].empty() || absoluteValueFailsGreaterThan1Check(values["angle"].as<long double>())) {
            std::cout<<"Enter a value for the angle x [radian]: ";
            std::cin >> input;
            try {
                replace(values, "angle", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }

        while(values["convergence-threshold"].empty() || failsPositiveNumberCheck(values["convergence-threshold"].as<long double>())) {
            std::cout<<"Enter a value for the stopping criterion e [e > 0]: ";
            std::cin >> input;
            try {
                replace(values, "convergence-threshold", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }

        while(values["iterations"].empty() || failsNaturalNumberCheck(values["iterations"].as<long double>())) {
            std::cout<<"Enter maximum number of iterations (natural number): ";
            std::cin >> input;
            try {
                replace(values, "iterations", asNumber(input));
            } catch (const std::exception &) {
                continue;
            }
        }
    }

    /**
     * @brief This function prints the input values.
     *
     * @param vm A map of variable names to their values.
     *
     * It retrieves the precision, angle, convergence threshold and iterations from the variable map and prints them.
     */
    void printInputArguments(boost::program_options::variables_map &vm) override {
        const auto precision = vm["precision"].as<int>();
        std::cout<<std::setw(44)<<"Inputs\n";
        CommandLine::printLine();
        std::cout << "\tangle, x: "<<std::setprecision(precision) << vm["angle"].as<long double>()<< "\n";
        std::cout << "\tconvergence-threshold, t: "<<std::setprecision(precision) << vm["convergence-threshold"].as<long double>()<< "\n";
        std::cout << "\titerations, n: "<<std::setprecision(default_precision) << vm["iterations"].as<long double>()<< "\n";
        CommandLine::printLine();
    }

    void buildInputs(TaylorSeriesInputs &inputs, boost::program_options::variables_map &values) override {
        inputs.x = values["angle"].as<long double>();
        inputs.target_threshold = values["convergence-threshold"].as<long double>();
        inputs.N = static_cast<size_t>(ceil(values["iterations"].as<long double>()));
    }
};
