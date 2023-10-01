/**
 * @file outlab5.cpp
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the declaration for the OutLab5 class.
 *
 */

#ifndef NE591_008_INLAB6_H
#define NE591_008_INLAB6_H

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
 * @class InLab6
 * @brief This class is a child of the Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class InLab6 : public Project<InputMatrices, Parser, RelaxationMethodOutputs> {

public:
    /**
     * @brief Constructor for the outlab5 class
     * @param args Command line arguments
     */
    explicit InLab6(CommandLineArgs args) : Project(args) {}

protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        auto x = -0.03;
        auto y = 0.77;
        auto iterations = 337;
        canvas.x_start = -0.396934597844;
        canvas.x_stop = -0.085211129213;
        canvas.y_start = 0.414181225609;
        canvas.y_stop = 0.632387653651;

        canvas.tone_map.growth_rate = 0.25;
     //   printJuliaSet<__float128>(canvas, x, y, iterations); //"o█■"
        std::cout<<"Julia set at ("<<x<<","<<y<<"), "<<iterations<<" iterations\n";
        return {
                .ProjectName = "NE591: InLab 06",
                .ProjectDescription = "Solving a system of linear equations using the Point Jacobi method",
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
    void run(RelaxationMethodOutputs &outputs, InputMatrices &inputs, boost::program_options::variables_map &values) override {

        /**
         * TODO:: Document
        **/

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);

        const auto A = inputs.coefficients;
        const auto b = inputs.constants;

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_POINT_JACOBI)) {
            RelaxationMethodOutputs pointJacobiResults;
            pointJacobiResults.inputs = inputs;
            Compute::usingPointJacobi(pointJacobiResults);
            pointJacobiResults.toJSON(results[MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_POINT_JACOBI)]);
            Parser::printLine();
            std::cout<<"Point Seidel Method Results"<<std::endl;
            Parser::printLine();
            std::cout<<"\ttotal iterations          : "<<(pointJacobiResults.solution.iterations)<<std::endl;
            std::cout<<"\tconverged                 : "<<(pointJacobiResults.solution.converged ? "Yes" : "No")<<std::endl;
            std::cout<<"\titerative error           : "<<(pointJacobiResults.solution.iterative_error)<<std::endl;
            std::cout<<"\tabsolute maximum residual : "<<(pointJacobiResults.getMaxResidual())<<std::endl;
            std::cout<<"\texecution time (ns)       : "<<(pointJacobiResults.getMaxResidual())<<std::endl;
        }

//        outputs.solution = x;
//
//        const auto b_prime = A * x;
//        const auto r = b - b_prime;
//        outputs.residual = r;
//
//        const auto maxResidual = outputs.getMaxResidual();
//
//        outputs.toJSON(results["outputs"]);
//
//        if(!values.count("quiet")) {
//            const auto precision = getTerminal().getCurrentPrecision();
//
//            Parser::printLine();
//            std::cout << "Lower Triangular Matrix (L):\n";
//            Parser::printLine();
//            std::cout << std::scientific << std::setprecision(precision) << L;
//            Parser::printLine();
//            std::cout << "Upper Triangular Matrix (U):\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << U;
//            Parser::printLine();
//            if (pivot) {
//                std::cout << "Permutation Matrix (P):\n";
//                Parser::printLine();
//                std::cout << std::setprecision(precision) << P;
//                Parser::printLine();
//                std::cout << "Permuted constants (Pb = P * b):\n";
//                Parser::printLine();
//                std::cout << std::setprecision(precision) << Pb;
//                Parser::printLine();
//            }
//            std::cout << "Intermediate vector (y), where (Ly = "<< (pivot ? "Pb" : "b") << "):\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << y;
//            Parser::printLine();
//            std::cout << "Solution vector (x), where (Ux = y):\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << x;
//            Parser::printLine();
//            std::cout << "Residual vector (r = b - Ax) :\n";
//            Parser::printLine();
//            std::cout << std::setprecision(precision) << r;
//            Parser::printLine();
//            std::cout << "Max Residual abs(r): ";
//            std::cout << std::setprecision(max_precision) << maxResidual << std::endl;
//            Parser::printLine();
//        }
//
        writeJSON(values["output-json"].as<std::string>(), results);
    }

};

#endif // NE591_008_INLAB6_H
