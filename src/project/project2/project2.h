/**
 * @file project2.cpp
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief This file contains the declaration for the Project2 class.
 *
 */

#ifndef NE591_008_PROJECT2_PROJECT2_H
#define NE591_008_PROJECT2_PROJECT2_H

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
#include "math/blas/Stats.h"
#include "math/blas/matrix/Matrix.h"
#include "math/factorization/LU.h"

#include "Compute.h"
#include "json.hpp"

/**
 * @class Project2
 * @brief This class is a child of the Project class and is used to solve a system of linear equations using forward and
 * back substitution.
 * @details The class takes in command line arguments and uses them to solve the system of equations.
 */
class Project2 : public Project<SolverInputs, Parser, SolverOutputs> {

  public:
    /**
     * @brief Constructor for the project2 class
     * @param args Command line arguments
     */
    explicit Project2(CommandLineArgs args) : Project(args) {}

  protected:
    /**
     * @brief This function builds the header information for the project.
     * @return HeaderInfo object containing project information
     */
    HeaderInfo buildHeaderInfo() override {
        Canvas canvas;
        auto x = -0.172;
        auto y = -0.66;
        auto iterations = 2000;
        canvas.x_start = -0.007514104707;
        canvas.x_stop = 0.075446744304;
        canvas.y_start = 0.825578589953;
        canvas.y_stop = 0.883651184261;
        printJuliaSet<__float128>(canvas, x, y, iterations); //"o█■"
        std::cout << "Julia set at (" << x << "," << y << "), " << iterations << " iterations\n";
        return {
            .ProjectName = "NE591: Project Milestone 2",
            .ProjectDescription = "Iterative, Lazy-Evaluated, Serial Diffusion Equation Solver",
            .SubmissionDate = "10/13/2023",
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
        //std::cout << "\trelative error            : " << (results()) << std::endl;
//        std::cout << "\tabsolute maximum residual : "
//                  << (results.solution.getMaxResidual(results.inputs.input.coefficients,
//                                                      results.inputs.input.constants))
//                  << std::endl;
        //        std::cout << "\texecution time [ns]       : " << formatMeanStd(results.mean_execution_time, results.stddev_execution_time) << std::endl;
        std::cout << "\texecution time [ms]       : " << formatMeanStd(results.mean_execution_time/1.0e6, results.stddev_execution_time/1.0e6) << std::endl;
        //        std::cout << "\texecution time [s]        : " << formatMeanStd(results.mean_execution_time/1.0e9, results.stddev_execution_time/1.0e9) << std::endl;
    }


    /**
     * @brief This function runs the project.
     * @details It solves the system of linear equations using forward and back substitution.
     * @param outputs The output vector
     * @param inputs The input matrices
     * @param values The variable map
     */
    void run(SolverOutputs &outputs, SolverInputs &inputs, boost::program_options::variables_map &values) override {

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

        if (inputs.methods.count(MyFactorizationMethod::Type::METHOD_LUP)) {
            SolverOutputs runResults(inputs);
            Compute::usingLUP(runResults, inputs);
            runResults.toJSON(results["outputs"][MyFactorizationMethod::TypeKey(MyFactorizationMethod::Type::METHOD_LUP)]);
            Parser::printLine();
            std::cout << "LUP Factorization Results" << std::endl;
            Parser::printLine();
            printResults(runResults);
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_POINT_JACOBI)) {
            SolverOutputs runResults(inputs);
            Compute::usingPointJacobi(runResults, inputs);
            runResults.toJSON(results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_POINT_JACOBI)]);
            Parser::printLine();
            std::cout << "Point Jacobi Method Results" << std::endl;
            Parser::printLine();
            printResults(runResults);
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_SORJ)) {
            SolverOutputs runResults(inputs);
            Compute::usingJacobiSOR(runResults, inputs);
            runResults.toJSON(results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_SORJ)]);
            Parser::printLine();
            std::cout << "SOR Point Jacobi Method Results" << std::endl;
            Parser::printLine();
            printResults(runResults);
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_GAUSS_SEIDEL)) {
            SolverOutputs runResults(inputs);
            Compute::usingGaussSeidel(runResults, inputs);
            runResults.toJSON(
                results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_GAUSS_SEIDEL)]);
            Parser::printLine();
            std::cout << "Gauss-Seidel Method Results" << std::endl;
            Parser::printLine();
            printResults(runResults);
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_SOR)) {
            SolverOutputs runResults(inputs);
            Compute::usingSOR(runResults, inputs);
            runResults.toJSON(results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_SOR)]);
            Parser::printLine();
            std::cout << "SOR Method Results" << std::endl;
            Parser::printLine();
            printResults(runResults);
        }

        if (inputs.methods.count(MyRelaxationMethod::Type::METHOD_SSOR)) {
            SolverOutputs runResults(inputs);
            Compute::usingSymmetricSOR(runResults, inputs);
            runResults.toJSON(results["outputs"][MyRelaxationMethod::TypeKey(MyRelaxationMethod::Type::METHOD_SSOR)]);
            Parser::printLine();
            std::cout << "Symmetric SOR Method Results" << std::endl;
            Parser::printLine();
            printResults(runResults);
        }

        Parser::printLine();

        // write output data
        if (values.count("output-results-json")) {
            outputs.toJSON(results["outputs"]);
            writeJSON(values["output-results-json"].as<std::string>(), results);
        }

        if (values.count("output-flux-csv")) {
            writeCSVMatrixNoHeaders(values["output-flux-csv"].as<std::string>(), outputs.fluxes);
        }

//        const bool quietMode = values.count("quiet");
//
//        std::vector<Stopwatch<Nanoseconds>> clocks = std::vector<Stopwatch<Nanoseconds>>(5);
//        std::vector<long double> durations = std::vector<long double>(5);
//        nlohmann::json profiler;
//
//        /**
//         * @brief Step 1: Initialize and fill the diffusion (coefficient) matrix (A) and constants vector (b)
//         */
//        clocks[0].restart();
//        {
//            for (size_t i = 0; i < 10; i++) {
//                //intermediates = initialize_diffusion_matrix_and_vector<long double>(inputs.m, inputs.n);
//                //naive_fill_diffusion_matrix_and_vector(inputs, intermediates);
//            }
//        }
//        clocks[0].click();
//        durations[0] = static_cast<long double>(clocks[0].duration().count()) / 10.0;
//        profiler["exclusive"]["initialize_and_fill"] = durations[0];
//        profiler["cumulative"]["initialize_and_fill"] = durations[0];
//
//        if (!quietMode) {
//            Parser::printLine();
//            std::cout << "Diffusion Matrix A: \n";
//            Parser::printLine();
//            //std::cout << intermediates.diffusion_matrix_A;
//            Parser::printLine();
//            std::cout << "Right Hand Side Vector B: \n";
//            Parser::printLine();
//            //std::cout << intermediates.right_hand_side_vector_B;
//        }
//
//        /**
//         * @brief Step 2: Solve the diffusion equation by computing the solution vector (x) for the given coefficients
//         * (A) and constants (b).
//         */
//        // Step 2.1 Perform LU factorization with pivoting
//        clocks[1].restart();
//        //{ naive_solve_linear_system(intermediates); }
//        clocks[1].click();
//        durations[1] = static_cast<long double>(clocks[1].duration().count());
//        profiler["exclusive"]["lup_factorize"] = durations[1];
//        profiler["cumulative"]["lup_factorize"] = durations[1] + durations[0];
//
//        // Step 2.2 Apply forward, backward substitution steps to get the solution (x) vector phi (fluxes).
//        MyBLAS::Vector<long double> b, Pb, y, phi;
//        clocks[2].restart();
//        {
//         //   b = intermediates.right_hand_side_vector_B;
//         //   Pb = intermediates.P * intermediates.right_hand_side_vector_B;
//         //   y = MyBLAS::forwardSubstitution<long double>(intermediates.L, Pb);
//         //   phi = MyBLAS::backwardSubstitution<long double>(intermediates.U, y);
//        }
//        clocks[2].click();
//        durations[2] = static_cast<long double>(clocks[2].duration().count());
//        profiler["exclusive"]["fb_substitution"] = durations[2];
//        profiler["cumulative"]["fb_substitution"] = durations[2] + durations[1] + durations[0];
//
//        profiler["exclusive"]["solve_linear_system"] = durations[2] + durations[1];
//        profiler["cumulative"]["solve_linear_system"] = durations[2] + durations[1] + durations[0];
//
//        /**
//         * @brief Step 3: Given the now computed solution vector (x or phi) calculate the flux in the 2D grid,
//         * accounting for the boundary conditions (+1 each side, each dimension).
//         */
//        MyBLAS::Vector<long double> b_prime, r;
//        long double maxResidual;
//        clocks[3].restart();
//        {
//            // compute fluxes
//            fill_fluxes(phi, inputs, outputs);
//           // b_prime = intermediates.diffusion_matrix_A * phi;
//           // r = b - b_prime;
//            outputs.residual = r;
//          //  outputs.solution = phi;
//
//            maxResidual = MyBLAS::Stats::max<long double>(MyBLAS::Stats::abs(r));
//        }
//        clocks[3].click();
//        durations[3] = static_cast<long double>(clocks[3].duration().count());
//        profiler["exclusive"]["residuals"] = durations[3];
//        profiler["cumulative"]["residuals"] = durations[3] + durations[2] + durations[1] + durations[0];
//
//        if (!values.count("quiet")) {
//            Parser::printLine();
//            std::cout << "Lower Triangular Matrix (L):\n";
//            Parser::printLine();
//           // std::cout << intermediates.L;
//            Parser::printLine();
//            std::cout << "Upper Triangular Matrix (U):\n";
//            Parser::printLine();
//           // std::cout << intermediates.U;
//            Parser::printLine();
//            std::cout << "Permutation Matrix (P):\n";
//            Parser::printLine();
//           // std::cout << intermediates.P;
//            Parser::printLine();
//            std::cout << "Permuted constants (Pb = P * b):\n";
//            Parser::printLine();
//            std::cout << Pb;
//            Parser::printLine();
//            std::cout << "Intermediate vector (y), where (Ly = Pb):\n";
//            Parser::printLine();
//            std::cout << y;
//            Parser::printLine();
//            std::cout << "Solution vector (x, phi), where (Ux = y):\n";
//            Parser::printLine();
//            std::cout << phi;
//            Parser::printLine();
//            Parser::printLine();
//            std::cout << "Computed flux 𝜙(𝑖,𝑗): \n";
//            Parser::printLine();
//            std::cout << outputs.fluxes;
//            Parser::printLine();
//            std::cout << "Residual vector (r = b - Ax) :\n";
//            Parser::printLine();
//            std::cout << r;
//            Parser::printLine();
//            std::cout << "Max Residual abs(r): ";
//            std::cout << std::setprecision(max_precision) << maxResidual << std::endl;
//            Parser::printLine();
//        }
//
//        clocks[4].restart();
//        {
//            // write json
//            if (values.count("output-results-json")) {
//                nlohmann::json results;
//                inputs.toJSON(results["inputs"]);
//                outputs.toJSON(results["outputs"]);
//                writeJSON(values["output-results-json"].as<std::string>(), results);
//            }
//
//            if (values.count("output-flux-csv")) {
//                writeCSVMatrixNoHeaders(values["output-flux-csv"].as<std::string>(), outputs.fluxes);
//            }
//        }
//        clocks[4].click();
//        durations[4] = static_cast<long double>(clocks[4].duration().count());
//        profiler["exclusive"]["post-process"] = durations[4];
//        profiler["cumulative"]["post-process"] =
//            durations[4] + durations[3] + durations[2] + durations[1] + durations[0];
//        profiler["total"] = durations[4] + durations[3] + durations[2] + durations[1] + durations[0];
//        profiler["problem_size"] = inputs.m * inputs.n;
//
//        writeJSON("profile_" + std::to_string(inputs.m) + "x" + std::to_string(inputs.n) + ".json", profiler);
//    }
};
};

#endif // NE591_008_PROJECT2_PROJECT2_H
