/**
 * @file Profiler.h
 * @author Arjun Earthperson
 * @date 10/11/2023
 * @brief TODO:: DOCUMENT
 */

#ifndef NE591_008_PROFILER_H
#define NE591_008_PROFILER_H

#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <utility>

#include "CheckBounds.h"
#include "Stopwatch.h"
#include "json.hpp"
#include "math/blas/Stats.h"

// TODO:: DOCUMENT
template <typename FunctionType>
class Profiler {
  public:

    // TODO:: DOCUMENT
    explicit Profiler(FunctionType function, size_t runs = 1, long double timeout = 0, std::string description = ""): _function(function) {
        _timedOut = false;
        _totalRuns = runs > 0 ? runs : 1;
        _timeout = timeout > 0 ? timeout : 0;
        _stopwatches = std::vector<Stopwatch<Nanoseconds>>(0);
        _description = description;
        _summary = MyBLAS::Stats::Summary<long double>();
    }

    // TODO:: DOCUMENT
    Profiler &run() {
        resetRuns();
        if (_timeout > 0) {
            _timedOut = runWithTimeout();
        } else {
            runNoTimeout();
            _timedOut = false;
        }
        summarize(_summary, _stopwatches);
        return *this;
    }

    // TODO:: DOCUMENT
    friend std::ostream &operator<<(std::ostream &os, const Profiler &m) {
        os << R"(::::::::::::::::::::::::::::::: PROFILE SUMMARY ::::::::::::::::::::::::::::::::)"<<std::endl;
        os << "["<<m._stopwatches.size()<<"/"<<m._totalRuns<<"] : "<<m._description<<std::endl;
        os << R"(::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::)"<<std::endl;
        os << m._summary << std::endl;
        os << R"(::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::)"<<std::endl;
        return os;
    }

    // TODO:: DOCUMENT
    const MyBLAS::Stats::Summary<long double> &getSummary() const { return _summary; }

  private:
    // TODO:: DOCUMENT
    FunctionType _function;

    // TODO:: DOCUMENT
    size_t _totalRuns;

    // TODO:: DOCUMENT
    long double _timeout;

    // TODO:: DOCUMENT
    bool _timedOut;

    std::string _description;

    // TODO:: DOCUMENT
    std::vector<Stopwatch<Nanoseconds>> _stopwatches;

    MyBLAS::Stats::Summary<long double> _summary;

    // TODO:: DOCUMENT
    void runNoTimeout() {
        for(size_t i = 0; i < _totalRuns; i++) {
            auto stopwatch = Stopwatch<Nanoseconds>().restart();
            _function();
            stopwatch.click();
            _stopwatches.emplace_back(stopwatch);
        }
    }

    // TODO:: DOCUMENT
    bool runWithTimeout() {
        auto timeoutWatch = Stopwatch<Nanoseconds>();
        timeoutWatch.restart();

        for(size_t i = 0; i < _totalRuns; i++) {
            // timed out
            if(timeoutWatch.peek_elapsed_time().count() > _timeout) {
                _timedOut = true;
                break;
            }
            // run the calculation
            auto stopwatch = Stopwatch<Nanoseconds>().restart();
            _function();
            stopwatch.click();
            _stopwatches.emplace_back(stopwatch);
        }

        return _timedOut;
    }

    // TODO:: DOCUMENT
    void resetRuns() {
        _timedOut = false;
        _stopwatches.clear();
        _stopwatches.resize(0);
        _summary = MyBLAS::Stats::Summary<long double>();
    }

    // TODO:: DOCUMENT
    static void summarize(MyBLAS::Stats::Summary<long double> &summary, const std::vector<Stopwatch<Nanoseconds>> &stopwatches) {
        // Create an array of durations
        MyBLAS::Vector<long double> durations(stopwatches.size());
        std::transform(stopwatches.begin(), stopwatches.end(), durations.getData().begin(), [](const Stopwatch<Nanoseconds>& stopwatch) { return stopwatch.duration().count(); });

        // Calculate statistics
        summary.min = MyBLAS::Stats::min(durations);
        summary.max = MyBLAS::Stats::max(durations);
        summary.sum = MyBLAS::Stats::sum(durations);
        summary.mean = MyBLAS::Stats::mean(durations);
        summary.variance = MyBLAS::Stats::variance(durations);
        summary.stddev = MyBLAS::Stats::std(durations);
        summary.median = MyBLAS::Stats::median(durations);
        summary.p5th = MyBLAS::Stats::percentile(durations, 5);
        summary.P95th = MyBLAS::Stats::percentile(durations, 95);
    }
};

#endif // NE591_008_PROFILER_H
