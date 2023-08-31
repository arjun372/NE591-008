/**
 * @file outlab1.cpp
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Entrypoint for outlab1
 */

#include <boost/program_options.hpp>

#include "utils/CommandLine.h"
#include "Compute.h"
#include "ProcessInputs.h"

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
            .scalar = inputs.k,
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
    std::cout<<std::setw(40)<<"Outputs\n";
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
 * It sets up the program information, builds the input options, parses the command line arguments, checks the input
 * values, prints the input values, runs the main logic of the program, and then returns the exit status.
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
