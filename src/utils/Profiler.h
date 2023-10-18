/**
* @file Profiler.h
* @author Arjun Earthperson
* @date 10/11/2023
* @brief This file contains the Profiler class which is used to profile the execution time of a function.
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

/**
* @brief A template class to profile the execution time of a function.
* @tparam FunctionType The type of the function to be profiled.
*/
template <typename FunctionType>
class Profiler {
 public:

   /**
    * @brief Constructor for the Profiler class.
    * @param function The function to be profiled.
    * @param runs The number of times the function should be run for profiling.
    * @param timeout The maximum time allowed for the function to run.
    * @param description A description of the function being profiled.
    */
   explicit Profiler(FunctionType function, size_t runs = 1, long double timeout = 0, std::string description = ""): _function(function) {
       _timedOut = false;
       _totalRuns = runs > 0 ? runs : 1;
       _timeout = timeout > 0 ? timeout : 0;
       _stopwatches = std::vector<Stopwatch<Nanoseconds>>(0);
       _description = description;
       _summary = MyBLAS::Stats::Summary<long double>();
   }

   // TODO:: DOCUMENT
   size_t getTotalRuns() const { return _totalRuns; }

   /**
    * @brief Runs the function for profiling and summarizes the results.
    * @return A reference to this Profiler object.
    */
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

   /**
    * @brief Overloads the << operator to print the summary of the profiling.
    * @param os The output stream to print to.
    * @param m The Profiler object to print.
    * @return The output stream.
    */
   friend std::ostream &operator<<(std::ostream &os, const Profiler &m) {
       os << R"(::::::::::::::::::::::::::::::: PROFILE SUMMARY ::::::::::::::::::::::::::::::::)"<<std::endl;
       os << "["<<m._stopwatches.size()<<"/"<<m._totalRuns<<"] : "<<m._description<<std::endl;
       os << R"(::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::)"<<std::endl;
       os << m._summary << std::endl;
       os << R"(::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::)"<<std::endl;
       return os;
   }

   /**
    * @brief Gets the summary of the profiling.
    * @return The summary of the profiling.
    */
   const MyBLAS::Stats::Summary<long double> &getSummary() {
       _summary.runs = _stopwatches.size();
       return _summary;
   }

 private:
   FunctionType _function; ///< The function to be profiled.
   size_t _totalRuns; ///< The number of times the function should be run for profiling.
   long double _timeout; ///< The maximum time allowed for the function to run.
   bool _timedOut; ///< Whether the function timed out during profiling.
   std::string _description; ///< A description of the function being profiled.
   std::vector<Stopwatch<Nanoseconds>> _stopwatches; ///< A vector of stopwatches to time each run of the function.
   MyBLAS::Stats::Summary<long double> _summary; ///< The summary of the profiling.

   /**
    * @brief Runs the function for profiling without a timeout.
    */
   void runNoTimeout() {
       for(size_t i = 0; i < _totalRuns; i++) {
           auto stopwatch = Stopwatch<Nanoseconds>().restart();
           _function();
           stopwatch.click();
           _stopwatches.emplace_back(stopwatch);
       }
   }

   /**
    * @brief Runs the function for profiling with a timeout.
    * @return Whether the function timed out during profiling.
    */
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

   /**
    * @brief Resets the runs for profiling.
    */
   void resetRuns() {
       _timedOut = false;
       _stopwatches.clear();
       _stopwatches.resize(0);
       _summary = MyBLAS::Stats::Summary<long double>();
   }

   /**
    * @brief Summarizes the results of the profiling.
    * @param summary The summary to store the results in.
    * @param stopwatches The stopwatches used to time each run of the function.
    */
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
       summary.p95th = MyBLAS::Stats::percentile(durations, 95);
   }
};

#endif // NE591_008_PROFILER_H
