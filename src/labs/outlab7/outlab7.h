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
#include "MPIProject.h"

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
     * @brief Constructor for the outlab7 class
     * @param args Command line arguments
     */
    explicit OutLab7(CommandLineArgs args) : MPIProject<OutLab7Inputs, Parser, OutLab7Outputs>(args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
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

    /**
     * @brief This function runs the computation.
     * @param outputs The outputs of the computation.
     * @param inputs The inputs to the computation.
     * @param values The values used in the computation.
     */
    void run(OutLab7Outputs &outputs, OutLab7Inputs &inputs, boost::program_options::variables_map &values) override {

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);

//        int numprocs, rank_id;
//        MPI_Status stat;
//
//        auto args = getTerminal().getCmdArgs();
//        MPI_Init(&args.argc, &args.argv);
//        MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
//        MPI_Comm_rank(MPI_COMM_WORLD, &rank_id);
////
//        auto n = 1000;
//        MPI_Bcast(&n, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);
//
//        if ((numprocs & (numprocs - 1)) != 0) {
//            if (rank_id == 0) {
//                std::cout << "Number of processes is not a power of 2";
//            }
//            MPI_Finalize();
//            exit(1);
//        }
//
//        size_t i1 = n / numprocs * rank_id;
//        size_t i2 = rank_id == numprocs - 1 ? n : n / numprocs * (rank_id + 1);
//
//        MyBLAS::NumericType partial_sum = 0;
//        for (size_t i = i1; i < i2; i++) {
//            partial_sum += (1 + log10(i)) / pow(i + log10(i + 1), 2);
//        }
//
//        int dest = rank_id % 2 == 0 ? rank_id + 1 : rank_id - 1;
//        MPI_Send(&partial_sum, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
//
//        if (rank_id == 0) {
//            double total_sum;
//            MPI_Recv(&total_sum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &stat);
//            std::cout << "The value of the series is: " << total_sum << std::endl;
//        }
        Parser::printLine();
        outputs.toJSON(results["outputs"]);
        writeJSON(values["output-json"].as<std::string>(), results);
    }
};

#endif // NE591_008_OUTLAB7_H
