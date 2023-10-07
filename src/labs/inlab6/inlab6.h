/**
 * @file inlab6.cpp
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief This file contains the declaration for the InLab6 class.
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
#include "math/factorization/LU.h"

#include "json.hpp"
#include "Compute.h"


/**
 * @class InLab6
 * @brief This class is a child of the Project class
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class InLab6 : public Project<InLab6Inputs, Parser, InLab6Outputs> {

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
        auto x = -0.292;
        auto y = -0.66;
        auto iterations = 200;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        canvas.tone_map.growth_rate = 0.3;
        printJuliaSet<__float128>(canvas, x, y, iterations); //"o█■"
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
     * @brief This function prints the results of the computation.
     * @param results The results of the computation.
     */
    static void printResults(InLab6Outputs &results) {
        std::cout<<"\ttotal iterations          : "<<(results.solution.iterations)<<std::endl;
        std::cout<<"\tconverged                 : "<<(results.solution.converged ? "Yes" : "No")<<std::endl;
        std::cout<<"\titerative error           : "<<(results.solution.iterative_error)<<std::endl;
        std::cout<<"\tabsolute maximum residual : "<<(results.getMaxResidual())<<std::endl;
        std::cout<<"\texecution time (ns)       : "<<(results.execution_time)<<std::endl;
        std::cout<<"\texecution time (ms)       : "<<(results.execution_time/1.0e6)<<std::endl;
        std::cout<<"\texecution time (s)        : "<<(results.execution_time/1.0e9)<<std::endl;
    }

    /**
     * @brief This function runs the project.
     * @details It solves the system of linear equations using forward and back substitution.
     * @param outputs The output vector
     * @param inputs The input matrices
     * @param values The variable map
     */
    void run(InLab6Outputs &outputs, InLab6Inputs &inputs, boost::program_options::variables_map &values) override {

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_POINT_JACOBI)) {
            InLab6Outputs pointJacobiResults(inputs);
            Compute::usingPointJacobi(pointJacobiResults, inputs);
            pointJacobiResults.toJSON(results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_POINT_JACOBI)]);
            Parser::printLine();
            std::cout<<"Point Seidel Method Results"<<std::endl;
            Parser::printLine();
            printResults(pointJacobiResults);
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_GAUSS_SEIDEL)) {
            Parser::printLine();
            std::cout<<"Gauss Seidel not implemented yet."<<std::endl;
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_SOR)) {
            Parser::printLine();
            std::cout<<"Successive over-relaxation (SOR) not implemented yet."<<std::endl;
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_SSOR)) {
            Parser::printLine();
            std::cout<<"Symmetric Successive over-relaxation (SOR) not implemented yet."<<std::endl;
        }

        Parser::printLine();
        writeJSON(values["output-json"].as<std::string>(), results);
    }

};

#endif // NE591_008_INLAB6_H
