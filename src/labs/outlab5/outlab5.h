/**
 * @file outlab5.cpp
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the declaration for the OutLab5 class.
 *
 */

#ifndef NE591_008_OUTLAB5_H
#define NE591_008_OUTLAB5_H

#include <variant>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>

#include "InputsOutputs.h"
#include "Parser.h"

#include "Project.h"
#include "CommandLine.h"

#include "math/blas/MyBLAS.h"
#include "math/blas/Matrix.h"
#include "math/blas/LU.h"

#include "json.hpp"
#include "Compute.h"


/**
 * @class OutLab5
 * @brief This class is a child of the Project class and is used to solve a system of linear equations using forward and back substitution.
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class OutLab5 : public Project<InputMatrices, Parser, Results> {

public:
    /**
     * @brief Constructor for the outlab4 class
     * @param args Command line arguments
     */
    explicit OutLab5(CommandLineArgs args) : Project(args) {}

protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        std::cout<<"Julia set at (-0.9, 0.26), 200 iterations\n";
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

        // Given the matrix A = LU and vector b
        // solve Ax = b, which is LUx = b
        // x = inv(LU)•b

        auto A = inputs.coefficients;
        auto L = MyBLAS::Matrix(A.getRows(), A.getCols(), 0.0f);
        auto U = MyBLAS::Matrix(A.getRows(), A.getCols(), 0.0f);

        MyBLAS::Matrix P;

        factorizeLUP<long double>(L, U, A, P);

        if(!MyBLAS::isUnitLowerTriangularMatrix(L)) {
            std::cerr << "Factorized matrix L is not unit lower triangular, aborting.\n";
            exit(-1);
        }

        if(!MyBLAS::isUpperTriangularMatrix(U)) {
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
            std::cout << std::scientific << std::setprecision (precision) << L;
            Parser::printLine();
            std::cout << "Upper Triangular Matrix (U):\n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << U;
            Parser::printLine();
            std::cout << "Factorized Matrix LU: \n";
            Parser::printLine();
            std::cout << std::setprecision (precision) << L + U - MyBLAS::Matrix::eye(A.getCols());
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

#endif // NE591_008_OUTLAB5_H
