/**
 * @file Profiler.h
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief This file contains helper classes and functions for profiling.
 */

#ifndef NE591_008_PROFILER_H
#define NE591_008_PROFILER_H

#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <utility>

#include "Stopwatch.h"

// TODO:: DOCUMENT
struct ProfilerOptions {
    size_t runs;
    size_t timeout;
};

/**
 * @brief A class that provides methods for profiling the execution time of functions.
 */
class Profiler {
  public:

    explicit Profiler(ProfilerOptions opts) {
        options = std::move(opts);
    }

    explicit Profiler() {
        options.runs = 1;
        options.timeout = 0;
    }

    explicit Profiler(size_t runs) {
        options.runs = runs;
    }

    // TODO:: DOCUMENT
    size_t tick() {
        return 1;
    }

    // t
    size_t tock() {
        return 0;
    }

  private:
    ProfilerOptions options;
    //Stopwatch<Nanoseconds> stopwatch; ///< A Stopwatch object for measuring time in microseconds.



  public:

    // TODO:: DOCUMENT
    static boost::program_options::options_description buildCommandlineOptions() {
        boost::program_options::options_description profiler("Benchmarks");
        profiler.add_options()
                ("bench-runs,R", boost::program_options::value<long double>()->default_value(1), "= <R> runs to perform")
                    ("bench-timeout,T", boost::program_options::value<long double>()->default_value(0), "= Timeout after <T> seconds [0=never]")
                        ("bench-output,O", boost::program_options::value<std::string>(), "= path for the benchmark output JSON");
        return profiler;
    }

    // TODO:: DOCUMENT
    // validate the input options, prompting the user if they need to be fixed
    static void validateOptions(boost::program_options::variables_map &map) {

    }

    // TODO:: DOCUMENT
    static ProfilerOptions Options(boost::program_options::variables_map &map) {
        validateOptions(map);
        ProfilerOptions options = {
            .runs = static_cast<size_t>(map["bench-runs"].as<long double>()),
            .timeout = static_cast<size_t>(map["bench-timeout"].as<long double>()),
            //.ostream = std::ofstream(map["bench-output"].as<std::string>()),
        };`11
        return options;
    }

    // TODO:: DOCUMENT
    static Profiler Init(boost::program_options::variables_map &map) {
        return Profiler(Profiler::Options(map));
    }
};

#endif // NE591_008_PROFILER_H
