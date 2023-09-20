/**
 * @file outlab4.cpp
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief This file contains the declaration for the OutLab4 class.
 *
 */

#pragma once

#include <variant>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>

#include "utils/Helpers.h"
#include "utils/CheckBounds.h"
#include "utils/FileParser.h"
#include "utils/Project.h"

#include "Parser.h"
#include "Compute.h"
#include "InputsOutputs.h"
#include "utils/json.hpp"

#include "utils/math/blas/Matrix.h"
#include "utils/math/blas/Vector.h"
#include "utils/math/blas/MyBLAS.h"


/**
 * @class OutLab4
 * @brief This class is a child of the Project class and is used to solve a system of linear equations using forward and back substitution.
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class OutLab4 : public Project<MyBLAS::InputMatrices, Parser, MyBLAS::OutputVector> {

public:
    /**
     * @brief Constructor for the outlab4 class
     * @param args Command line arguments
     */
    explicit OutLab4(CommandLineArgs args) : Project(args) {}

protected:

    /**
     * @brief This function prints the Mandelbrot set.
     * @details The function was converted from Python to C++ and reformatted for the ANSI character-set with 80 columns.
     * @note Credit to eyemvh from https://www.reddit.com/r/asciiart/comments/eyemvh/asciibrot/
     */
    template <typename T>
    static void printMandelbrotSet() {
        std::vector<std::string> colors;
        for (int i = 16; i <= 232; i++) {
            colors.push_back("\033[38;5;" + std::to_string(i) + "m");
        }
        double xs = -2.25, ys = -1.5, dx = 0.044, dy = 0.12;
        for (int y = 0; y < 25; ++y) {
            for (int x = 0; x < 80; ++x) {
                std::complex<T> z1 = std::complex<T>(xs + x * dx, ys + y * dy);
                std::complex<T> zn = z1;
                size_t c = 0;
                for (; c < 1000; c++) {
                    zn = zn * zn + z1;
                    if (std::norm(zn) > 8) break;
                }
                std::cout << colors[c % colors.size()] << "o";//"■";
            }
            std::cout << '\n';
        }
        std::cout << "\033[0m";
    }

    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        printMandelbrotSet<long double>();
        return {
                .ProjectName = "InLab 04",
                .ProjectDescription = "Solving a system of linear equations using forward, back substitution",
                .SubmissionDate = "09/15/2023",
                .StudentName = "Arjun Earthperson",
                .HeaderArt = " ",
        };
    }

    /**
     * @brief This function runs the project.
     * @details It solves the system of linear equations using forward and back substitution.
     * @param outputs The output vector
     * @param inputs The input matrices
     * @param values The variable map
     */
    void run(MyBLAS::OutputVector &outputs, MyBLAS::InputMatrices &inputs, boost::program_options::variables_map &values) override {

        // Given the matrix A = LU and vector b
        // solve Ax = b, which is LUx = b
        // x = inv(LU)•b

        const auto A = inputs.coefficients;
        auto L = MyBLAS::Matrix(A.getRows(), A.getCols(), 0.0f);
        auto U = MyBLAS::Matrix(A.getRows(), A.getCols(), 0.0f);

        MyBLAS::factorizeLU<long double>(L, U, A);

        const auto b = inputs.constants;

        const MyBLAS::Vector y = MyBLAS::forwardSubstitution<long double>(L, b);
        const MyBLAS::Vector x = MyBLAS::backwardSubstitution<long double>(U, y);

        nlohmann::json results;
        outputs.solution = x;
        outputs.toJSON(results["outputs"]);


        if(!values.count("quiet")) {
            const auto precision = getTerminal().getCurrentPrecision();
            Parser::printLine();
            std::cout << "Lower Triangular Matrix (L):\n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << L;
            Parser::printLine();
            std::cout << "Upper Triangular Matrix (U):\n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << U;
            Parser::printLine();
            std::cout << "Factorized Matrix LU: \n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << L + U - MyBLAS::Matrix::Eye(A.getCols());
            Parser::printLine();
            std::cout << "Intermediate vector y = inv(L) * b:\n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << y;
            Parser::printLine();
            std::cout << "Solution vector (x):\n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << x;
            Parser::printLine();
        }

        writeJSON(values["output-json"].as<std::string>(), results);
    }

};
