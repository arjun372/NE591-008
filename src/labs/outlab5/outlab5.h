/**
 * @file outlab5.cpp
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the declaration for the OutLab5 class.
 *
 */

#ifndef NE591_008_OUTLAB5_H
#define NE591_008_OUTLAB5_H

#include <boost/program_options.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <variant>

#include "InputsOutputs.h"
#include "Parser.h"

#include "CommandLine.h"
#include "Project.h"

#include "math/blas/BLAS.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/Stats.h"
#include "math/factorization/LU.h"

#include "Compute.h"
#include "json.hpp"

/**
 * @class OutLab5
 * @brief This class is a child of the Project class and is used to solve a system of linear equations using forward and
 * back substitution.
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class OutLab5 : public Project<InputMatrices, Parser, Results> {

  public:
    /**
     * @brief Constructor for the outlab5 class
     * @param args Command line arguments
     */
    explicit OutLab5(CommandLineArgs args) : Project(args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        auto x = -0.03;
        auto y = 0.77;
        size_t iterations = 200;
        canvas.x_start = -0.396934597844;
        canvas.x_stop = -0.085211129213;
        canvas.y_start = 0.414181225609;
        canvas.y_stop = 0.632387653651;
        canvas.tone_map.growth_rate = 0.20;
        printJuliaSet<__float128>(canvas, x, y, iterations); //"o█■"
        return {
            .ProjectName = "NE591: OutLab 05",
            .ProjectDescription = "Solving a system of linear equations using LUP factorization",
            .SubmissionDate = "09/29/2023",
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
    void run(Results &outputs, InputMatrices &inputs, boost::program_options::variables_map &values) override {

        /**
            1. Given the matrix A = LU and vector b, solve Ax = b, which is LUx = b.
            2. Perform row pivoting on A to obtain a permuted matrix PA = LU, where P is a permutation matrix.
            3. Let y = Ux. Now, we have two systems of linear equations: Ly = Pb and Ux = y.
            4. Solve the first system Ly = Pb using forward substitution.
            5. Solve the second system Ux = y using backward substitution.
        **/

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);

        const auto A = inputs.coefficients;

        auto L = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));
        auto U = MyBLAS::Matrix(A.getRows(), A.getCols(), static_cast<long double>(0));

        const auto pivot = !values.count("no-pivoting");
        const bool alternateMethod = values.count("alternate-method") != 0;
        MyBLAS::Matrix<long double> P;

        if (pivot) {
            if (alternateMethod) {
                P = MyBLAS::cast<long double, bool>(MyBLAS::cast<bool, long double>(dooLittleFactorizeLUP(L, U, A)));
            } else {
                P = MyBLAS::cast<long double, bool>(
                    MyBLAS::cast<bool, long double>(factorizeLUwithPartialPivoting(L, U, A)));
            }
        } else {
            MyBLAS::LU::factorize(L, U, A);
        }

        if (!MyBLAS::isUnitLowerTriangularMatrix(L)) {
            std::cerr << "Warning: Factorized matrix L is not unit lower triangular, expect undefined behavior.\n";
        }

        if (!MyBLAS::isUpperTriangularMatrix(U)) {
            std::cerr << "Warning: Factorized matrix U is not upper triangular, expect undefined behavior.\n";
        }

        if (pivot) {
            if (!MyBLAS::isPermutationMatrix(P)) {
                std::cerr << "Warning: Generated matrix P is not a permutation matrix, expect undefined behavior.\n";
            }
        }

        IntermediateResults intermediates;
        intermediates.L = L;
        intermediates.U = U;
        if (pivot) {
            intermediates.P = P;
        }
        intermediates.toJSON(results["intermediates"]);

        const auto b = inputs.constants;
        const auto Pb = pivot ? (P * b) : b;

        const MyBLAS::Vector y = MyBLAS::forwardSubstitution(L, Pb);
        const MyBLAS::Vector x = MyBLAS::backwardSubstitution(U, y);

        outputs.solution = x;

        const auto b_prime = A * x;
        const auto r = b - b_prime;
        outputs.residual = r;

        const auto maxResidual = MyBLAS::Stats::max<long double>(MyBLAS::Stats::abs(r));
        outputs.max_residual = maxResidual;

        outputs.toJSON(results["outputs"]);

        if (!values.count("quiet")) {
            const auto precision = getTerminal().getCurrentPrecision();

            Parser::printLine();
            std::cout << "Lower Triangular Matrix (L):\n";
            Parser::printLine();
            std::cout << std::scientific << std::setprecision(static_cast<int>(precision)) << L;
            Parser::printLine();
            std::cout << "Upper Triangular Matrix (U):\n";
            Parser::printLine();
            std::cout << std::setprecision(static_cast<int>(precision)) << U;
            Parser::printLine();
            if (pivot) {
                std::cout << "Permutation Matrix (P):\n";
                Parser::printLine();
                std::cout << std::setprecision(static_cast<int>(precision)) << P;
                Parser::printLine();
                std::cout << "Permuted constants (Pb = P * b):\n";
                Parser::printLine();
                std::cout << std::setprecision(static_cast<int>(precision)) << Pb;
                Parser::printLine();
            }
            std::cout << "Intermediate vector (y), where (Ly = " << (pivot ? "Pb" : "b") << "):\n";
            Parser::printLine();
            std::cout << std::setprecision(static_cast<int>(precision)) << y;
            Parser::printLine();
            std::cout << "Solution vector (x), where (Ux = y):\n";
            Parser::printLine();
            std::cout << std::setprecision(static_cast<int>(precision)) << x;
            Parser::printLine();
            std::cout << "Residual vector (r = b - Ax) :\n";
            Parser::printLine();
            std::cout << std::setprecision(static_cast<int>(precision)) << r;
            Parser::printLine();
            std::cout << "Max Residual abs(r): ";
            std::cout << std::setprecision(max_precision) << maxResidual << std::endl;
            Parser::printLine();
        }

        writeJSON(values["output-json"].as<std::string>(), results);
    }
};

#endif // NE591_008_OUTLAB5_H
