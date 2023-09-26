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

#include "CommandLine.h"
#include "Project.h"
#include "Compute.h"
#include "Parser.h"

typedef struct Output {

} NoOutput;

class OutLab1 : public Project<MatrixConstructParams , Parser, NoOutput> {

public:
    explicit OutLab1(CommandLineArgs args) : Project(args) {}

protected:
    HeaderInfo buildHeaderInfo() override {
        return HeaderInfo {
                .ProjectName = "OutLab 01",
                .ProjectDescription = "Non-vectorized, elementwise (mul, add) operations on 2D matrices",
                .SubmissionDate = "09/01/2023",
                .StudentName = "Arjun Earthperson",
                .HeaderArt = R"(
                                               _
                 ___                          (_)
               _/XXX\
_             /XXXXXX\_                                    __
X\__    __   /X XXXX XX\                          _       /XX\__      ___
    \__/  \_/__       \ \                       _/X\__   /XX XXX\____/XXX\
  \  ___   \/  \_      \ \               __   _/      \_/  _/  -   __  -  \__/
 ___/   \__/   \ \__     \\__           /  \_//  _ _ \  \     __  /  \____//
/  __    \  /     \ \_   _//_\___     _/    //           \___/  \/     __/
__/_______\________\__\_/________\_ _/_____/_____________/_______\____/_______
                                  /|\
                                 / | \
                                /  |  \
                               /   |   \
)",
        };
    }

    /**
     * @brief This function runs the main logic of the program.
     *
     * @param values A map of variables parsed from the command line arguments.
     *
     * The function first extracts the input parameters from the values map. It then builds and prints three matrices A,
     * B, and F using the input parameters. After that, it computes and prints three more matrices C, D, and E using the
     * previously built matrices and the scalar value from the input parameters.
     */
    void run(NoOutput &output, MatrixConstructParams &input, boost::program_options::variables_map &values) override {

        // build matrix
        ArgsPrintMatrix matrix{
                .start_row = 1, // spec says indexing starts from 1 not 0.
                .start_col = 1, // spec says indexing starts from 1 not 0.
                .num_rows = input.M,
                .num_cols = input.N,
                .inputParams = input,
                .evaluate = elementFromA,
        };
        printMatrix(matrix, "Matrix A: ");
        Parser::printLine();

        // build matrix B
        matrix.evaluate = elementFromB;
        printMatrix(matrix, "Matrix B: ");
        Parser::printLine();

        // build N x J matrix F
        matrix.num_rows = input.N;
        matrix.num_cols = input.J;
        matrix.evaluate = elementFromF;
        printMatrix(matrix, "Matrix F: ");
        Parser::printLine();

        // Now the computed values
        std::cout << std::setw(44) << "Outputs\n";
        Parser::printLine();

        // build M x N matrix C
        matrix.num_rows = input.M;
        matrix.num_cols = input.N;
        matrix.evaluate = elementFromC;
        printMatrix(matrix, "Matrix C = A+B");
        Parser::printLine();

        // build matrix D
        matrix.evaluate = elementFromD;
        printMatrix(matrix, "Matrix D = kA");
        Parser::printLine();

        // build matrix E
        matrix.num_rows = input.M;
        matrix.num_cols = input.J;
        matrix.evaluate = elementFromE;
        printMatrix(matrix, "Matrix E = AF");
        Parser::printLine();

    }
};
