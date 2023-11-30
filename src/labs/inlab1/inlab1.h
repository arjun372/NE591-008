/**
 * @file inlab1.cpp
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief Entrypoint for inlab1
 */

#pragma once

#include <functional>
#include <iomanip>
#include <iostream>

#include <boost/program_options.hpp>

#include "Compute.h"
#include "Parser.h"

#include "CheckBounds.h"
#include "CommandLine.h"
#include "Helpers.h"
#include "Project.h"
#include "utils/profiler/Profiler.h"
#include "utils/profiler/Stopwatch.h"

/**
 * @brief Struct to hold variables for Taylor Series.
 */
typedef struct Output TaylorSeriesVariables;

class InLab1 : public Project<TaylorSeriesInputs, Parser, TaylorSeriesVariables> {

  public:
    explicit InLab1(CommandLineArgs args) : Project(args) {}

  protected:
    HeaderInfo buildHeaderInfo() override {
        return HeaderInfo{
            .ProjectName = "InLab 01",
            .ProjectDescription = "Iterative Taylor series approximation of sin(x)",
            .SubmissionDate = "08/30/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = R"(
                                       .-'~~~-.
                                     .'o  oOOOo`.
                                    :~~~-.oOo   o`.
                                     `. \ ~-.  oOOo.
                                       `.; / ~.  OO:
                                       .'  ;-- `.o.'
                                      ,'  ; ~~--'~
                                      ;  ;
_____________________\|/__________\\;_\\//___\|/_______________________________)",
        };
    }

    void run(TaylorSeriesVariables &output, TaylorSeriesInputs &input,
             boost::program_options::variables_map &values) override {

        std::cout << std::setw(40) << "Profile\n";
        Parser::printLine();
        Stopwatch<Microseconds> clock;
        // my_sin(x)
        long double my_sin_val;
        clock.restart();
        for (int i = 0; i < 1000; i++) {
            my_sin_val = my_sin(static_cast<long double>(i) / 1000.0f, input.N, input.target_threshold);
        }
        clock.click();

        std::cout << " my_sin(x) completed in: " << (static_cast<long double>(clock.duration().count()) * 1.0f) << "ns"
                  << std::endl;
        auto my_sin_val2 = my_sin(input.x, input.N, input.target_threshold);
        // sin(x)
        long double math_sin_val;
        Stopwatch<Microseconds> clock2;
        clock2.restart();
        for (int i = 0; i < 1000; i++) {
            math_sin_val = std::sin(static_cast<long double>(i) / 1000.0f);
        }
        clock2.click();
        std::cout << " sin(x) completed in: " << (static_cast<long double>(clock2.duration().count()) * 1.0f) << "ns"
                  << std::endl;
        auto math_sin_val2 = std::sin(input.x);
        // truncation error
        const long double truncation_error = abs(math_sin_val2 - my_sin_val2);
        Parser::printLine();
        const auto precision = values["precision"].as<int>();
        std::cout << std::setw(44) << "Outputs\n";
        Parser::printLine();
        std::cout << "\tConverged at n=" << mySineVars.n
                  << " at convergence threshold: " << mySineVars.current_threshold << "\n\t...\n";
        std::cout << "\tmy_sin(x):" << std::setw(37) << std::setprecision(static_cast<int>(precision)) << my_sin_val2 << "\n";
        std::cout << "\tsin(x) from math.h:" << std::setw(28) << std::setprecision(static_cast<int>(precision)) << math_sin_val2 << "\n";
        std::cout << "\ttruncation error: " << std::setw(30) << std::setprecision(static_cast<int>(precision)) << truncation_error
                  << "\n";
        Parser::printLine();
        my_sin_val += my_sin_val;
        math_sin_val += math_sin_val;
    }
};
