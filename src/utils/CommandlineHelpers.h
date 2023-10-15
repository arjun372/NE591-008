/**
* @file CommandlineHelpers.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief This file contains helper functions for handling command line options and initializing the profiler.
 */


#ifndef NE591_008_COMMANDLINEHELPERS_H
#define NE591_008_COMMANDLINEHELPERS_H

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
#endif // NE591_008_COMMANDLINEHELPERS_H
