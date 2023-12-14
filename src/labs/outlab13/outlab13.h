/**
 * @file outlab13.cpp
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the declaration for the InLab13class.
 *
 */

#ifndef NE591_008_OUTLAB13_OUTLAB13_H
#define NE591_008_OUTLAB13_OUTLAB13_H

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
#include "math/Stats.h"
#include "math/blas/matrix/Matrix.h"
#include "math/factorization/LU.h"

#include "Compute.h"
#include "json.hpp"

/**
 * @class InLab13
 * @brief This class is a child of the Project class and is used to solve a system of linear equations using forward and
 * back substitution.
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class InLab13: public Project<SolverInputs, Parser, SolverOutputs> {

  public:
    /**
     * @brief Constructor for the outlab13 class
     * @param args Command line arguments
     */
    explicit InLab13(CommandLineArgs args) : Project(args) {

    }

  protected:
    /**
     * @brief This function builds the header information for the labs.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        auto x = -0.172;
        auto y = -0.66;
        const size_t iterations = 2000;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        printJuliaSet<__float128>(canvas, x, y, iterations); //"o█■"
        return {
            .ProjectName = "NE591: OutLab 13",
            .ProjectDescription = "Newton Raphson's Method - Diffusion Equation Solver",
            .SubmissionDate = "11/20/2023",
            .StudentName = "Arjun Earthperson",
            .HeaderArt = " ",
        };
    }

    /**
     * @brief This function prints the results of the computation.
     * @param results The results of the computation.
     */
    static void printResults(SolverOutputs &results) {
        std::cout << "\torder                     : " << (results.inputs.solverParams.n) << std::endl;
        std::cout << "\ttotal iterations          : " << (results.solution.iterations) << std::endl;
        std::cout << "\tconverged                 : " << (results.solution.converged ? "Yes" : "No") << std::endl;
        std::cout << "\titerative error           : " << (results.solution.iterative_error) << std::endl;
        std::cout << "\tabsolute maximum residual : " << (results.max_residual()) << std::endl;
    }


    /**
     * @brief This function runs the project.
     * @details It solves the system of linear equations using forward and back substitution.
     * @param output The output vector
     * @param inputs The input matrices
     * @param values The variable map
     */
    void run(SolverOutputs &output, SolverInputs &inputs, boost::program_options::variables_map &values) override {

        /**
            1. Read input parameters: Read the values of 𝑎, 𝑏, 𝑚, 𝑛, 𝐷, and Σₐ from an input file. Also,
            read the non-uniformly distributed fixed source 𝑞(𝑖,𝑗) for 𝑖 = 1, … , 𝑚 and 𝑗 = 1, … , 𝑛 from the input
        file.

            2. Calculate mesh spacings: Compute the mesh spacings 𝛿 and 𝛾 using the formulas 𝛿 = 𝑎/(𝑚+1) and 𝛾 =
        𝑏/(𝑛+1).

            3. Initialize the matrix and right-hand-side vector: Create an 𝑚×𝑛 matrix A and an 𝑚×𝑛 right-hand-side
            vector B. Initialize all elements of A and B to zero.

            4. Fill the matrix A and vector B: Loop through all the nodes 𝑖 = 1, … , 𝑚 and 𝑗 = 1, … , 𝑛, and fill the
            matrix A and vector B using the given equation:

                   −D((φ(i+1,j) - 2φ(i,j) + φ(i-1,j))/δ² + (φ(i,j+1) - 2φ(i,j) + φ(i,j-1))/γ²) + Σₐφ(i,j) = q(i,j)

            Note that the boundary conditions are already taken into account as the fluxes at the boundaries are zero.

            5. Solve the linear system: Solve the linear system A * 𝜙 = B using LU factorization with pivoting.
            The solution vector 𝜙 will contain the scalar fluxes at each node 𝑖, 𝑗, where 𝑖 = 1, … , 𝑚 and 𝑗 = 1, … , 𝑛.

            6. Output the results: Write the scalar fluxes 𝜙(𝑖,𝑗) for 𝑖 = 1, … , 𝑚 and 𝑗 = 1, … , 𝑛 to an output file.
        **/

        /**
         * @brief This section of the function handles the computation using different methods.
         * @details The methods include LUP Factorization, Point Jacobi, Gauss-Seidel, SOR, SOR Point Jacobi, and SSOR.
         */

        nlohmann::json results;
        inputs.toJSON(results["inputs"]);


        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_FIXED_POINT)) {
            SolverOutputs runResults(inputs);
            Compute::usingFixedPoint(runResults, inputs);
            runResults.toJSON(results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_FIXED_POINT)]);
//            Parser::printLine();
//            std::cout << "Fixed Point Method Results" << std::endl;
            //Parser::printLine();
//            printResults(runResults);
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_NEWTON)) {
            SolverOutputs runResults(inputs);
            Compute::usingNewtons(runResults, inputs);
            runResults.toJSON(results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_NEWTON)]);
//            Parser::printLine();
            //std::cout << "Point Jacobi Method Results" << std::endl;
//            Parser::printLine();
//            printResults(runResults);
        }


        Parser::printLine();

        // write output data
        if (values.count("output-results-json")) {
            writeJSON(values["output-results-json"].as<std::string>(), results);
        }
};
};

#endif // NE591_008_OUTLAB13_OUTLAB13_H
