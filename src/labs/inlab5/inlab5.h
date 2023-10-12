/**
 * @file inlab5.cpp
 * @author Arjun Earthperson
 * @date 09/22/2023
 * @brief This file contains the declaration for the InLab5 class.
 *
 */

#pragma once

#include <boost/program_options.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <variant>

#include "CheckBounds.h"
#include "FileParser.h"
#include "Helpers.h"
#include "Project.h"

#include "Compute.h"
#include "InputsOutputs.h"
#include "Parser.h"
#include "json.hpp"

#include "math/blas/BLAS.h"
#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"

/**
 * @class InLab5
 * @brief This class is a child of the Project class and is used to solve a system of linear equations using forward and
 * back substitution, with pivoting
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class InLab5 : public Project<MyBLAS::InputMatrices, Parser, MyBLAS::OutputVector> {

  public:
    /**
     * @brief Constructor for the inlab5 class
     * @param args Command line arguments
     */
    explicit InLab5(CommandLineArgs args) : Project(args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        printJuliaSet<__float128>(canvas, -0.8, 0.156, 300); //"o█■"
        std::cout << "\t\t\tJulia set at (-0.8, 0.156), 300 iterations\n";
        return {
            .ProjectName = "InLab 05",
            .ProjectDescription = "Linear equations with permutation matrix, forward, back substitution",
            .SubmissionDate = "09/22/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = " ",
        };
    }

    /**
     * @brief This function runs the project.
     * @details It solves the system of linear equations using forward and back substitution, with pivoting
     * @param outputs The output vector
     * @param inputs The input matrices
     * @param values The variable map
     */
    void run(MyBLAS::OutputVector &outputs, MyBLAS::InputMatrices &inputs,
             boost::program_options::variables_map &values) override {

        // PAx = Pb, or LUx = P

        const auto b = inputs.constants;
        const auto LU = inputs.LU;
        const auto P = inputs.permutation;

        const auto Pb = P * b;

        const MyBLAS::Vector y = MyBLAS::forwardSubstitution<long double>(LU, Pb);
        const MyBLAS::Vector x = MyBLAS::backwardSubstitution<long double>(LU, y);

        nlohmann::json results;
        outputs.solution = x;
        outputs.toJSON(results["outputs"]);

        if (!values.count("quiet")) {
            const auto precision = getTerminal().getCurrentPrecision();
            std::cout << "Permuted constants Pb = P * b:\n";
            Parser::printLine();
            std::cout << std::setprecision(precision) << Pb;
            Parser::printLine();
            std::cout << "Intermediate vector y = inv(L) * b:\n";
            Parser::printLine();
            std::cout << std::setprecision(precision) << y;
            Parser::printLine();
            std::cout << "Solution vector (x):\n";
            Parser::printLine();
            std::cout << std::setprecision(precision) << x;
            Parser::printLine();
        }

        writeJSON(values["output-json"].as<std::string>(), results);
    }
};
