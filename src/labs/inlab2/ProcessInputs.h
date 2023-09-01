/**
 * @file ProcessInputs.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Process inputs for inlab2
 */

#pragma once

#include <iostream>
#include <iomanip>

#include "utils/Helpers.h"
#include "utils/CheckBounds.h"


typedef struct InputParams {
    size_t n;
    size_t m;
    size_t x;
    std::vector<long double> fx;
} InputParams;

/**
 * @brief This function builds the input options for the program.
 *
 * @return A boost::program_options::options_description object containing the description of the input options.
 */
boost::program_options::options_description buildInputs() {
    boost::program_options::options_description arguments("Parameters");
    arguments.add_options()
            ("num-points,n", boost::program_options::value<long double>(), "= (optional) number of interpolation points n")
            ("num-lip-points,m", boost::program_options::value<long double>(), "= number of Lagrange interpolation evaluation points")
            ("points,x", boost::program_options::value<std::vector<long double>>(), "= distinct, sorted interpolation points")
            ("fx-file,i", boost::program_options::value<std::string>(), "= path for plaintext file listing f(x=n) points")
            ("use-fx-function,f", boost::program_options::value<long double>(), "= use bundled fx function");
    return arguments;
}

/**
 * @brief This function performs checks on the input parameters and prompts the user to enter valid inputs if the
 * current inputs are invalid.
 *
 * @param values A boost::program_options::variables_map object containing the input values to be checked.
 */
static void performInputChecks(boost::program_options::variables_map &values) {

    std::string input;
    while (values["num-points"].empty() || failsNaturalNumberCheck(values["num-points"].as<long double>())) {
        std::cout << "Enter the number of interpolation points: ";
        std::cin >> input;
        try {
            replace(values, "num-points", asNumber(input));
        } catch (const std::exception &) {
            continue;
        }
    }

    while ((values["m-rank"].empty() || failsNaturalNumberCheck(values["m-rank"].as<long double>()))) {
        std::cout << "Enter a value for M, which is the rank for [A], and row rank for [B]: ";
        std::cin >> input;
        try {
            replace(values, "m-rank", asNumber(input));
        } catch (const std::exception &) {
            continue;
        }
    }

    while ((values["n-rank"].empty() || failsNaturalNumberCheck(values["n-rank"].as<long double>()))) {
        std::cout << "Enter a value for N, which is the column rank for [B], and row rank for [F]: ";
        std::cin >> input;
        try {
            replace(values, "n-rank", asNumber(input));
        } catch (const std::exception &) {
            continue;
        }
    }

    while ((values["j-rank"].empty() || failsNaturalNumberCheck(values["j-rank"].as<long double>()))) {
        std::cout << "Enter a value for J, which is the column rank for [F]: ";
        std::cin >> input;
        try {
            replace(values, "j-rank", asNumber(input));
        } catch (const std::exception &) {
            continue;
        }
    }
}

/**
 * @brief This function prints the input values.
 *
 * @param vm A boost::program_options::variables_map object containing the input values to be printed.
 *
 */
void printInputs(boost::program_options::variables_map &vm) {

    // retrieve the inputs
    const auto precision = vm["precision"].as<int>();
    const auto k = vm["scalar"].as<long double>();
    const auto M = static_cast<size_t>(vm["m-rank"].as<long double>());
    const auto N = static_cast<size_t>(vm["n-rank"].as<long double>());
    const auto J = static_cast<size_t>(vm["j-rank"].as<long double>());

    // list the parameters
    std::cout << std::setw(44) << "Inputs\n";
    CommandLine::printLine();
    std::cout << "\tscalar, k: " << std::setprecision(precision) << k << "\n";
    std::cout << "\tm-rank, M: " << M << "\n";
    std::cout << "\tn-rank, N: " << N << "\n";
    std::cout << "\tj-rank, J: " << J<< "\n";
    CommandLine::printLine();
}
