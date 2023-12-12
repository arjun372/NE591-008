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

#include "InputsOutputs.h"
#include "Parser.h"

#include "CommandLine.h"
#include "Project.h"

#include "Compute.h"
#include "json.hpp"

/**
 * @class InLab10
 * @brief This class is a child of the MPI Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class InLab10 : public Project<InLab10Inputs, Parser, InLab10Outputs> {

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
    explicit InLab10(CommandLineArgs args) : Project(args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
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
     * @brief This function is used to perform the main computation.
     * @details The computation is divided among the processes. Each process computes a partial sum and then the partial
     * sums are combined using a binary tree communication logic.
     * @param outputs The outputs of the computation.
     * @param inputs The inputs to the computation.
     * @param rank The rank of the current process.
     * @param size The total number of processes.
     * @return Returns false to indicate that the computation was successful.
     */
    void run(InLab10Outputs &outputs, InLab10Inputs &inputs, boost::program_options::variables_map &values) override {

        auto profiler = getProfiler([&inputs, &outputs]{
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


        outputs.summary = profiler.getSummary();
        std::cout<<profiler<<std::endl;

        std::cout << outputs << std::endl;

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);
        outputs.toJSON(results["outputs"]);
        writeJSON(inputs.outputJSON, results);
    }
};

#endif // NE591_008_INLAB10_H
