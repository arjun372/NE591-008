/**
 * @file inlab10.cpp
 * @author Arjun Earthperson
 * @date 10/27/2023
 * @brief This file contains the declaration for the InLab10 class.
 *
 */

#ifndef NE591_008_INLAB10_H
#define NE591_008_INLAB10_H

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
 * @class InLab10
 * @brief This class is a child of the MPI Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class InLab10 : public MPIProject<InLab10Inputs, Parser, InLab10Outputs> {

  public:
    /**
     * @brief This function is used to get the instance of the InLab10 class.
     * @details This function follows the Singleton design pattern. It ensures that only one instance of the InLab10
     * class is created.
     * @param args Command line arguments
     * @return Returns the instance of the InLab10 class.
     */
    [[maybe_unused]] static InLab10& getInstance(CommandLineArgs args) {
        static InLab10 instance(args);
        return instance;
    }

    /**
     * @brief This function is used to delete the copy constructor.
     * @details This function ensures that the InLab10 class cannot be copied. This is necessary because we are
     * following the Singleton design pattern.
     * @param InLab10 const& The reference to the InLab10 object to be copied.
     */
    InLab10(InLab10 const&) = delete;

    /**
     * @brief This function is used to delete the assignment operator.
     * @details This function ensures that the InLab10 class cannot be assigned. This is necessary because we are
     * following the Singleton design pattern.
     * @param InLab10 const& The reference to the InLab10 object to be assigned.
     */
    void operator=(InLab10 const&) = delete;

  protected:
    /**
     * @brief Constructor for the inlab10 class
     * @param args Command line arguments
     */
    explicit InLab10(CommandLineArgs args) : MPIProject<InLab10Inputs, Parser, InLab10Outputs>(buildHeaderInfo(), args) {}

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
            .ProjectName = "NE591: InLab 10",
            .ProjectDescription = "Matrix-Vector Operations Setup",
            .SubmissionDate = "10/27/2023",
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
    bool preRun(InLab10Outputs &outputs, InLab10Inputs &inputs, const int rank, const int size) override {
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
    bool run(InLab10Outputs &outputs, InLab10Inputs &inputs, const int rank, const int size) override {

        auto profiler = getProfiler([&rank, &inputs, &size, &outputs]{
            // A, y
            auto A = inputs.coefficients;
            auto y = inputs.constants;
            // z = Cy
            auto z = static_cast<MyBLAS::NumericType>(2.0) * y;
            // sum = y + z
            auto sum = y + z;
            // product = y^T • z
            auto product = y * z;
            // matrix weighted inner product = y^T • A•z
            auto matrix_weighted_inner_product = y * (A * z);

            outputs.scaled_vector = z;
            outputs.vector_sum = sum;
            outputs.scalar_product = product;
            outputs.matrix_inner_product = matrix_weighted_inner_product;
        }, 10000, 0, "Matrix-Vector Operations").run();

        if (rank == 0) {
            outputs.summary = profiler.getSummary();
            std::cout<<profiler<<std::endl;
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
    bool postRun(InLab10Outputs &outputs, InLab10Inputs &inputs, const int rank, const int size) override {

        if (rank != 0) {
            return false;
        }

        std::cout << outputs << std::endl;

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);
        outputs.toJSON(results["outputs"]);
        writeJSON(inputs.outputJSON, results);
        return false;
    }
};

#endif // NE591_008_INLAB10_H