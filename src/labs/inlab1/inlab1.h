/**
 * @file inlab1.cpp
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief Entrypoint for inlab1
 */

#pragma once

#include <iostream>
#include <iomanip>
#include <functional>

#include <boost/program_options.hpp>

#include "CommandLineParser.h"
#include "Compute.h"

#include "utils/Project.h"
#include "utils/CheckBounds.h"
#include "utils/CommandLine.h"
#include "utils/Stopwatch.h"
#include "utils/Helpers.h"
#include "utils/Profiler.h"


/**
 * @brief Struct to hold variables for Taylor Series.
 */
typedef struct Output TaylorSeriesVariables;

class InLab1 : public Project<TaylorSeriesInputs , CommandLineParser, TaylorSeriesVariables> {

public:
    explicit InLab1(CommandLineArgs args) : Project(args) {}

protected:

    HeaderInfo buildHeaderInfo() override {
        return HeaderInfo {
                .ProjectName = "InLab 01",
                .ProjectDescription = "Iterative Taylor series approximation of sin(x)",
                .SubmissionDate = "08/30/2023",
                .StudentName = "Arjun Earthperson",
        };
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
    void run(Output &output, Input &input, boost::program_options::variables_map &values) override {

        std::cout << std::setw(40) << "Profile\n";
        CommandLineParser::printLine();
        Stopwatch<Microseconds> clock;
        // my_sin(x)
        long double my_sin_val;
        clock.restart();
        for(int i = 0; i < 1000; i++) {
            my_sin_val = my_sin(static_cast<long double>(i)/1000.0f, input.N, input.target_threshold);
        }
        clock.click();

        std::cout << " my_sin(x) completed in: " << static_cast<long double>(clock.duration().count()*1.0f) << "ns" << std::endl;
        auto my_sin_val2 = my_sin(input.x, input.N, input.target_threshold);
        // sin(x)
        long double math_sin_val;
        Stopwatch<Microseconds> clock2;
        clock2.restart();
        for(int i = 0; i < 1000; i++) {
            math_sin_val = sin(static_cast<long double>(i)/1000.0f);
        }
        clock2.click();
        std::cout << " sin(x) completed in: " << static_cast<long double>(clock2.duration().count()*1.0f) << "ns" << std::endl;
        auto math_sin_val2 = sin(input.x);
        // truncation error
        const long double truncation_error = abs(math_sin_val2 - my_sin_val2);
        CommandLineParser::printLine();
        const auto precision = values["precision"].as<int>();
        std::cout<<std::setw(44)<<"Outputs\n";
        CommandLineParser::printLine();
        std::cout << "\tConverged at n="<<mySineVars.n<<" at convergence threshold: "<<mySineVars.current_threshold<<"\n\t...\n";
        std::cout << "\tmy_sin(x):"<<std::setw(37)<<std::setprecision(precision) << my_sin_val2<< "\n";
        std::cout << "\tsin(x) from math.h:"<<std::setw(28)<<std::setprecision(precision) << math_sin_val2<< "\n";
        std::cout << "\ttruncation error: "<<std::setw(30)<<std::setprecision(precision) << truncation_error << "\n";
        CommandLineParser::printLine();
        my_sin_val += my_sin_val;
        math_sin_val += math_sin_val;
    }

};
