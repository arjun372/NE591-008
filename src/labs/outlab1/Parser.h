/**
 * @file ProcessInputs.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Process inputs for outlab1
 */

#pragma once

#include <iostream>
#include <iomanip>

#include "CommandLine.h"
#include "Helpers.h"
#include "CheckBounds.h"

/**
 * @brief A structure to hold the input parameters.
 *
 * @param k A long double representing the scalar multiplier for [A].
 * @param M A size_t representing the row rank for [A] and [B].
 * @param N A size_t representing the column rank for [B] and row rank for [F].
 * @param J A size_t representing the column rank for [F].
 */
typedef struct Input {
    long double k;
    size_t M;
    size_t N;
    size_t J;
} MatrixConstructParams;

class Parser : public CommandLine<MatrixConstructParams> {

public:
    explicit Parser(const HeaderInfo &headerInfo, const CommandLineArgs &args) : CommandLine(headerInfo, args) {

    }

    explicit Parser() = default;

protected:
    /**
     * @brief This function builds the input options for the program.
     */
    void buildInputArguments(boost::program_options::options_description &arguments) override {
        arguments.add_options()
                ("scalar,k", boost::program_options::value<long double>(), "= scalar multiplier for [A]")
                ("m-rank,M", boost::program_options::value<long double>(), "= row rank for [A], row rank for [B]")
                ("n-rank,N", boost::program_options::value<long double>(), "= column rank for [B], row rank for [F]")
                ("j-rank,J", boost::program_options::value<long double>(), "= column rank for [F]");
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
        while (values["scalar"].empty()) {
            std::cout << "Enter a value for k, which is the scalar multiplier for [A]: ";
            std::cin >> input;
            try {
                replace(values, "scalar", asNumber(input));
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
     * @param vm A map of variable names to their values.
     *
     * It retrieves the precision, angle, convergence threshold and iterations from the variable map and prints them.
     */
    void printInputArguments(boost::program_options::variables_map &vm) override {
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

    void buildInputs(MatrixConstructParams &ToFill, boost::program_options::variables_map &values) override {
        ToFill.k = values["scalar"].as<long double>();
        ToFill.M = static_cast<size_t>(values["m-rank"].as<long double>());
        ToFill.N = static_cast<size_t>(values["n-rank"].as<long double>());
        ToFill.J = static_cast<size_t>(values["j-rank"].as<long double>());
    }
};
