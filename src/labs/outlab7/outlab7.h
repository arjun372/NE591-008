/**
 * @file outlab7.cpp
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the declaration for the OutLab7 class.
 *
 */

#ifndef NE591_008_OUTLAB7_H
#define NE591_008_OUTLAB7_H

#include <boost/program_options.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <variant>
#include <mpi.h>

#include "InputsOutputs.h"
#include "Parser.h"

#include "CommandLine.h"
#include "utils/mpi/MPIProject.h"

#include "Compute.h"
#include "json.hpp"

/**
 * @class OutLab7
 * @brief This class is a child of the MPI Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class OutLab7 : public MPIProject<OutLab7Inputs, Parser, OutLab7Outputs> {

  public:
    // TODO:: DOCUMENT
    [[maybe_unused]] static OutLab7& getInstance(CommandLineArgs args) {
        static OutLab7 instance(args);
        return instance;
    }

    // TODO:: DOCUMENT
    OutLab7(OutLab7 const&) = delete;
    void operator=(OutLab7 const&) = delete;

  protected:
    /**
     * @brief Constructor for the outlab7 class
     * @param args Command line arguments
     */
    explicit OutLab7(CommandLineArgs args) : MPIProject<OutLab7Inputs, Parser, OutLab7Outputs>(buildHeaderInfo(), args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    static HeaderInfo buildHeaderInfo() {
        Canvas canvas;
        __float128 x = 0.13;
        __float128 y = -0.66;
        auto iterations = 80;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        canvas.tone_map.growth_rate = 0.25;
        return {
            .ProjectName = "NE591: OutLab 07",
            .ProjectDescription = "",
            .SubmissionDate = "10/13/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = drawJuliaSet(canvas, x, y, iterations),
        };
    }

    // TODO:: DOCUMENT
    bool preRun(OutLab7Outputs &outputs, OutLab7Inputs &inputs, const int rank, const int size) override {
        if (failsPowerOf2Check(size)) {
            if (rank == 0) {
                std::cerr << "Number of processes is not a power of 2";
            }
            return true;
        }
        return false;
    }

    // TODO:: DOCUMENT
    bool run(OutLab7Outputs &outputs, OutLab7Inputs &inputs, const int rank, const int size) override {

        auto profiler = getProfiler([&rank, &inputs, &size, &outputs]{
            size_t start = static_cast<size_t>(rank) * inputs.n / static_cast<size_t>(size) + 1;
            size_t stop = static_cast<size_t>(rank + 1) * inputs.n / static_cast<size_t>(size);

            MyBLAS::NumericType partial_sum = 0;
            MyBLAS::NumericType previousLog = std::log10(start);

            for (size_t i = start + 1; i < stop; i++) {
                const MyBLAS::NumericType currentLog = std::log10(i);
                const MyBLAS::NumericType denominator = (i - 1) + currentLog;
                const MyBLAS::NumericType numerator = 1.0f + previousLog;
                previousLog = currentLog;
                const MyBLAS::NumericType term = std::pow(numerator / denominator, 2);
                partial_sum += term;
            }

            outputs.sum = partial_sum;

            int tag = 0;
            MPI_Status status;
            // Binary tree communication logic
            for (int stride = 1; stride < size; stride *= 2) {
                if (rank % (2 * stride) != 0) {
                    // Send partial sum to the process at an interval of stride
                    MPI_Send(&outputs.sum, 1, MPI_LONG_DOUBLE, rank - stride, tag, MPI_COMM_WORLD);
                    break;
                }
                // Receive partial sum from the process at an interval of stride
                if (rank + stride < size) {
                    MyBLAS::NumericType received_sum;
                    MPI_Recv(&received_sum, 1, MPI_LONG_DOUBLE, rank + stride, tag, MPI_COMM_WORLD, &status);
                    outputs.sum += received_sum;
                }
            }
        }, 10, 0, "run").run();

        if (rank == 0) {
            outputs.summary = profiler.getSummary();
            std::cout<<outputs.summary<<std::endl;
        }
        return false;
    }

    // TODO:: DOCUMENT
    bool postRun(OutLab7Outputs &outputs, OutLab7Inputs &inputs, const int rank, const int size) override {

        if (rank != 0) {
            return false;
        }

        // Print the final sum in the root process
        std::cout << _uuid << "Final sum: " << outputs.sum << std::endl;

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);
        outputs.toJSON(results["outputs"]);
        writeJSON(inputs.outputJSON, results);
        return false;
    }
};

#endif // NE591_008_OUTLAB7_H
