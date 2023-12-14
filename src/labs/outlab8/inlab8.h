/**
 * @file inlab8.cpp
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief This file contains the declaration for the OutLab7 class.
 *
 */

#ifndef NE591_008_INLAB8_H
#define NE591_008_INLAB8_H

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
    /**
     * @brief This function is used to get the instance of the OutLab7 class.
     * @details This function follows the Singleton design pattern. It ensures that only one instance of the OutLab7
     * class is created.
     * @param args Command line arguments
     * @return Returns the instance of the OutLab7 class.
     */
    [[maybe_unused]] static OutLab7& getInstance(CommandLineArgs args) {
        static OutLab7 instance(args);
        return instance;
    }

    /**
     * @brief This function is used to delete the copy constructor.
     * @details This function ensures that the OutLab7 class cannot be copied. This is necessary because we are
     * following the Singleton design pattern.
     * @param OutLab7 const& The reference to the OutLab7 object to be copied.
     */
    OutLab7(OutLab7 const&) = delete;

    /**
     * @brief This function is used to delete the assignment operator.
     * @details This function ensures that the OutLab7 class cannot be assigned. This is necessary because we are
     * following the Singleton design pattern.
     * @param OutLab7 const& The reference to the OutLab7 object to be assigned.
     */
    void operator=(OutLab7 const&) = delete;

  protected:
    /**
     * @brief Constructor for the inlab8 class
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
        const __float128 x = 0.13;
        const __float128 y = -0.66;
        const size_t iterations = 80;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        canvas.tone_map.growth_rate = 0.25;
        return {
            .ProjectName = "NE591: InLab 08",
            .ProjectDescription = "",
            .SubmissionDate = "10/13/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = drawJuliaSet<__float128>(canvas, x, y, iterations),
        };
    }

    /**
     * @brief Used to check if the number of processes is a power of 2 before running the main computation.
     * @details If the number of processes is not a power of 2, an error message is printed and the function returns
     * true to indicate an error.
     * @param outputs The outputs of the computation.
     * @param inputs The inputs to the computation.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return Returns true if the number of processes is not a power of 2, false otherwise.
     */
    bool preRun(OutLab7Outputs &outputs, OutLab7Inputs &inputs, const int rank, const int size) override {
        if (failsPowerOf2Check(size)) {
            if (rank == 0) {
                std::cerr << "Number of processes is not a power of 2";
            }
            return true;
        }
        return false;
    }

    /**
     * @brief This function is used to perform the main computation.
     * @details The computation is divided among the processes. Each process computes a partial sum and then the partial
     * sums are combined using a binary tree communication logic.
     * @param outputs The outputs of the computation.
     * @param inputs The inputs to the computation.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return Returns false to indicate that the computation was successful.
     */
    bool run(OutLab7Outputs &outputs, OutLab7Inputs &inputs, const int rank, const int size) override {

        auto profiler = getProfiler([&rank, &inputs, &size, &outputs]{

            size_t start = static_cast<size_t>(rank) * inputs.n / static_cast<size_t>(size) + 1;
            size_t stop = static_cast<size_t>(rank + 1) * inputs.n / static_cast<size_t>(size);

            outputs.sum  = Compute::seriesSum(start, stop);

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

    /**
     * @brief This function is used to perform operations after the main computation.
     * @details The final sum is printed in the root process and the inputs and outputs are written to a JSON file.
     * @param outputs The outputs of the computation.
     * @param inputs The inputs to the computation.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return Returns false to indicate that the post-run operations were successful.
     */
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

#endif // NE591_008_INLAB8_H
