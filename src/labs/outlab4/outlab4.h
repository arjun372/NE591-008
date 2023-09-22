/**
 * @file outlab4.cpp
 * @author Arjun Earthperson
 * @date 09/22/2023
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
     * @brief Scales a value from one range to another.
     * @tparam T The data type of the value and range limits
     * @param value The value to be scaled
     * @param old_min The lower limit of the original range
     * @param old_max The upper limit of the original range
     * @param new_min The lower limit of the new range
     * @param new_max The upper limit of the new range
     * @return The scaled value in the new range
     */
    template<typename T>
    static T scale(T value, T old_min, T old_max, T new_min, T new_max) {
        return (value - old_min) * (new_max - new_min) / (old_max - old_min) + new_min;
    }

    /**
     * @brief Prints a Julia set fractal to the console using ANSI color codes.
     * @tparam T The data type of the complex numbers used in the fractal calculation
     * @param X The width of the fractal in characters
     * @param Y The height of the fractal in characters
     * @param x0 The real part of the complex constant c (default: -1.0)
     * @param y0 The imaginary part of the complex constant c (default: -0.3)
     * @param max_iterations The maximum number of iterations for the fractal calculation (default: 112)
     * @param contrast The contrast factor for the fractal visualization (default: 1.0)
     */
    template <typename T>
    static void printJuliaSet(const size_t X, const size_t Y, const T x0 = -1.0, const T y0 = -0.3, const size_t max_iterations = 112, const long double contrast = 1.0) {
        T xs = -1.0, ys = -0.86, dx = 80.0 * 0.03 / static_cast<T>(X), dy = 25.0 * 0.08 / static_cast<T>(Y);
        std::complex<T> c(x0, y0); // You can change these values for different Julia sets
        T min = 10000.0f;
        T max = 0.0f;
        T avg = 0.0f;
        for (size_t y = 0; y < Y; ++y) {
            for (size_t x = 0; x < X; ++x) {
                std::complex<T> z = std::complex<T>(xs + x * dx, ys + y * dy);
                size_t iter = 0;
                for (; iter < max_iterations; iter++) {
                    z = z * z + c;
                    if (std::norm(z) > 4.0)
                        break;
                }
                if (std::log(iter) == NAN || std::log(iter) == std::log(0)) {
                    continue;
                }
                if (std::log(iter) < min) {
                    min = std::log(iter);
                }
                if (std::log(iter) > max) {
                   max = std::log(iter);
                }
                avg += std::log(iter);
            }
        }
        avg = avg / (X*Y);

        // ANSI color codes for shades of gray
        std::vector<std::string> colors3;
        std::vector<std::string> grayShades;
        grayShades.emplace_back("\033[38;5;0m");
        // Generate the color codes
        for (int i = 0; i < 24; ++i) {
            int colorCode = 232 + i;
            grayShades.push_back("\033[38;5;" + std::to_string(colorCode) + "m");
        }
        grayShades.emplace_back("\033[38;5;231m");
        colors3 = grayShades;
        for (size_t y = 0; y < Y; ++y) {
            for (size_t x = 0; x < X; ++x) {
                std::complex<T> z = std::complex<T>(xs + x * dx, ys + y * dy);
                size_t iter = 0;
                for (; iter < max_iterations; iter++) {
                    z = z * z + c;
                    if (std::norm(z) > 4.0)
                        break;
                }
                const auto itr = std::log(iter) == (std::log(iter) == NAN || std::log(iter) == std::log(0)) ? 0 : std::log(iter);
                long double enhanced_value = (itr - avg) * contrast + avg;
                auto colorc = enhanced_value;
                if (colorc < 0) {
                    colorc = 0;
                }
                colorc = scale<T>(colorc, min, max, 0, colors3.size());
                if (colorc > colors3.size() - 1) {
                    colorc = colors3.size() - 1;
                }
                if (colorc < 0) {
                    colorc = 0;
                }
                std::cout << colors3[static_cast<size_t>(colorc)] << "█";//"■";
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
        printJuliaSet<__float128>(80, 20, -0.9, 0.26, 300, 1.0);
        std::cout<<"Julia set at (-0.9, 0.26), 200 iterations\n";
        return {
                .ProjectName = "NE591: OutLab 04",
                .ProjectDescription = "Solving a system of linear equations using LU factorization",
                .SubmissionDate = "09/22/2023",
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

        if(!MyBLAS::isValidUnitLowerTriangularMatrix(L)) {
            std::cerr << "Factorized matrix L is not unit lower triangular, aborting.\n";
            exit(-1);
        }

        if(!MyBLAS::isValidUpperTriangularMatrix(U)) {
            std::cerr << "Factorized matrix U is not upper triangular, aborting.\n";
            exit(-1);
        }

        const auto b = inputs.constants;

        const MyBLAS::Vector y = MyBLAS::forwardSubstitution<long double>(L, b);
        const MyBLAS::Vector x = MyBLAS::backwardSubstitution<long double>(U, y);

        nlohmann::json results;
        outputs.solution = x;
        const auto b_prime = A * x;
        const auto r = MyBLAS::abs(b - b_prime);
        outputs.residual = r;
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
            std::cout << "Residual vector r=|b' - b| :\n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << r;
            Parser::printLine();
        }

        writeJSON(values["output-json"].as<std::string>(), results);
    }

};
