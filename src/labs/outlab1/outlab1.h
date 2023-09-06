/**
 * @file outlab1.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Entrypoint for outlab1, contains the OutLab1 implementation of LabProject
 *
 * The program performs non-vectorized, elementwise operations on 2D matrices. It uses the Boost library for parsing
 * command line arguments and handling program options. The main function sets up the program information, builds the
 * input options, parses the command line arguments, checks the input values, prints the input values, runs the main
 * logic of the program, and then returns the exit status.
 */

#pragma once

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"
#include "utils/Project.h"
#include "Compute.h"
#include "ProcessInputs.h"

class OutLab1 : public Project<>
/**
 * @brief This function runs the main logic of the program.
 *
 * @param values A map of variables parsed from the command line arguments.
 *
 * The function first extracts the input parameters from the values map. It then builds and prints three matrices A, B,
 * and F using the input parameters. After that, it computes and prints three more matrices C, D, and E using the
 * previously built matrices and the scalar value from the input parameters.
 */
static void run(boost::program_options::variables_map &values) {

    InputParams inputs = {
            .k = values["scalar"].as<long double>(),
            .M = static_cast<size_t>(values["m-rank"].as<long double>()),
            .N = static_cast<size_t>(values["n-rank"].as<long double>()),
            .J = static_cast<size_t>(values["j-rank"].as<long double>()),
    };
    // build matrix
    ArgsPrintMatrix matrix {
            .start_row = 1, // spec says indexing starts from 1 not 0.
            .start_col = 1, // spec says indexing starts from 1 not 0.
            .num_rows = inputs.M,
            .num_cols = inputs.N,
            .inputParams = inputs,
            .evaluate = elementFromA,
    };
    printMatrix(matrix, "Matrix A: ");
    CommandLine::printLine();

    // build matrix B
    matrix.evaluate = elementFromB;
    printMatrix(matrix, "Matrix B: ");
    CommandLine::printLine();

    // build N x J matrix F
    matrix.num_rows = inputs.N;
    matrix.num_cols = inputs.J;
    matrix.evaluate = elementFromF;
    printMatrix(matrix, "Matrix F: ");
    CommandLine::printLine();

    // Now the computed values
    std::cout<<std::setw(44)<<"Outputs\n";
    CommandLine::printLine();

    // build M x N matrix C
    matrix.num_rows = inputs.M;
    matrix.num_cols = inputs.N;
    matrix.evaluate = elementFromC;
    printMatrix(matrix, "Matrix C = A+B");
    CommandLine::printLine();

    // build matrix D
    matrix.evaluate = elementFromD;
    printMatrix(matrix, "Matrix D = kA");
    CommandLine::printLine();

    // build matrix E
    matrix.num_rows = inputs.M;
    matrix.num_cols = inputs.J;
    matrix.evaluate = elementFromE;
    printMatrix(matrix, "Matrix E = AF");
    CommandLine::printLine();

}


/**
 * @brief The main function of the program.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 *
 * @return The exit status of the program.
 *
 * The main function first sets up the program information and builds the input options. It then creates a CommandLine
 * object and uses it to parse the command line arguments into a map of variables. After that, it checks the input
 * values and prints them. Finally, it runs the main logic of the program and returns the exit status.
 */
int main(int argc, char **argv) {


    HeaderInfo programInfo{
            .ProjectName = "OutLab 01",
            .ProjectDescription = "Non-vectorized, elementwise (mul, add) operations on 2D matrices",
            .SubmissionDate = "09/01/2023",
            .StudentName = "Arjun Earthperson",
    };

    auto inputs = buildInputs();
    auto values = CommandLine(programInfo, inputs, argc, argv).getArguments();
    performInputChecks(values);
    printInputs(values);
    run(values);
    return 0;
}
