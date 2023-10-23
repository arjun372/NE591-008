/**
* @file ProfilerHelper.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief This file contains helper functions for handling command line options and initializing the profiler.
* @details The ProfilerHelper namespace provides a set of functions to build and validate command line options, and to
* initialize the profiler. The command line options are built using the boost::program_options library. The options
* include the number of runs to perform and the timeout duration. The options are then validated to ensure they meet
* the required conditions. If the options do not meet the conditions, the user is prompted to fix them. The profiler is
* then initialized with the given function, command line options, and description.
*/

#ifndef NE591_008_PROFILERHELPER_H
#define NE591_008_PROFILERHELPER_H

#include "CheckBounds.h"
#include "json.hpp"
#include <boost/program_options.hpp>

/**
 * @namespace ProfilerHelper
 * @brief This namespace contains helper functions for handling command line options and initializing the profiler.
 */
namespace ProfilerHelper {

/**
* @brief This function builds the command line options for the profiler.
* @details The function uses the boost::program_options library to define the command line options.
* The options include the number of runs to perform and the timeout duration.
* @return Returns a boost::program_options::options_description object containing the command line options.
 */
static boost::program_options::options_description buildCommandlineOptions() {
    boost::program_options::options_description profiler("Performance Benchmarking");
    profiler.add_options()
    ("bench-runs,R", boost::program_options::value<long double>()->default_value(1), "= <R> runs to perform")
    ("bench-timeout,T", boost::program_options::value<long double>()->default_value(0), "= Timeout after <T> seconds [0=never]");
    return profiler;
}

/**
* @brief This function validates the input options, prompting the user if they need to be fixed.
* @details The function reads the input json and populates the variables_map. It then performs checks on the parameters to ensure they meet the required conditions.
* If the parameters do not meet the conditions, the user is prompted to fix them.
* @param map A boost::program_options::variables_map object containing the command line options.
 */
static void validateOptions(boost::program_options::variables_map &map) {
    // read the input json and populate the variables_map
    nlohmann::json inputMap;
    std::vector<std::function<bool(long double)>> checks;
    // add checks for parameters a & b
    checks.clear();
    checks.emplace_back([](long double value) { return failsNaturalNumberCheck(value); });
    performChecksAndUpdateInput<long double>("bench-runs", inputMap, map, checks);
    checks.clear();
    checks.emplace_back([](long double value) { return failsNonNegativeNumberCheck(value); });
    performChecksAndUpdateInput<long double>("bench-timeout", inputMap, map, checks);
}

/**
* @brief This function initializes the profiler with the given function, command line options, and description.
* @details The function creates a Profiler object with the given function, number of runs, timeout duration, and description.
* The number of runs and timeout duration are retrieved from the command line options.
* @param func The function to be profiled.
* @param map A boost::program_options::variables_map object containing the command line options.
* @param description A string containing the description of the profiler.
* @return Returns a Profiler object.
 */
template <typename Func>
auto InitProfiler(Func func, boost::program_options::variables_map &map, std::string description) {
    auto runs = static_cast<size_t>(map["bench-runs"].as<long double>());
    auto timeout = map["bench-timeout"].as<long double>();
    return Profiler(func, runs, timeout, description);
}

}
#endif // NE591_008_PROFILERHELPER_H
