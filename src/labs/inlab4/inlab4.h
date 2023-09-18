/**
 * @file inlab4.cpp
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief Declaration for the InLab4 class
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

// TODO: Document
class InLab4 : public Project<myBLAS::InputMatrices, Parser, myBLAS::OutputVector> {

public:
    /**
     * @brief Constructor for the inlab4 class
     * @param args Command line arguments
     */
    explicit InLab4(CommandLineArgs args) : Project(args) {}

protected:

    // TODO: Document, make sure to give credit to eyemvh. this code was converted from python to c++ and reformatted
    // for the ANSI character-set with 80 columns.
    //      https://www.reddit.com/r/asciiart/comments/eyemvh/asciibrot/
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
     * @brief Builds the header information for the project
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


    // TODO: Document
    void run(myBLAS::OutputVector &outputs, myBLAS::InputMatrices &inputs, boost::program_options::variables_map &values) override {

        // Given the matrix A = LU and vector b
        // solve Ax = b, which is LUx = b
        // x = inv(LU)•b

        const auto L = inputs.lower;
        const auto U = inputs.upper;
        const auto b = inputs.constants;

        const myBLAS::Vector y = forwardSubstitution<long double>(L, b);
        const myBLAS::Vector x = backwardSubstitution<long double>(U, y);

        nlohmann::json results;
        outputs.x = x;
        outputs.toJSON(results["outputs"]);

        if(!values.count("quiet")) {
            const auto precision = getTerminal().getCurrentPrecision();
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
