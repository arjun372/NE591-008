/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief Compute methods for project milestone 1 in NE591-008.
 */

#ifndef NE591_008_PROJECT2_COMPUTE_H
#define NE591_008_PROJECT2_COMPUTE_H

#include <cmath>
#include <iostream>

#include "FileParser.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/MatrixVectorExpression.h"
#include "math/factorization/LU.h"
#include "math/factorization/LUP.h"
#include "math/relaxation/GaussSeidel.h"
#include "math/relaxation/PointJacobi.h"
#include "math/relaxation/SSOR.h"

/**
 * @namespace Compute
 * @brief Namespace containing methods for solving linear systems using various methods.
 */
namespace Compute {

/**
 * @brief A template function to fill a diffusion vector.
 * @details This function fills a diffusion vector using the fixed source q(i, j).
 * @tparam T The type of the diffusion constants.
 * @param inputs The solver inputs containing the diffusion parameters and sources.
 * @return The filled diffusion constants vector.
 */
template <typename T>
MyBLAS::Vector<T> &naive_fill_diffusion_vector(SolverInputs &inputs) {
    const size_t m = inputs.diffusionParams.getM();
    const size_t n = inputs.diffusionParams.getN();

    inputs.diffusionConstants = MyBLAS::Vector<T>(std::vector<T>(m * n, 0));

    // Loop through all the nodes i = 1, ..., m and j = 1, ..., n
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {

            // Calculate the index of the current node in the vector B
            const size_t idx = (i - 1) * n + (j - 1);

            // Fill the right-hand-side vector B using the fixed source q(i, j)
            inputs.diffusionConstants[idx] = inputs.sources[i - 1][j - 1];
        }
    }
    return inputs.diffusionConstants;
}

/**
 * @brief Fills the fluxes.
 *
 * This function fills the fluxes based on the given phi vector.
 * The fluxes are stored in a matrix with m+2 rows and n+2 columns.
 *
 * @param inputs The inputs for the solver.
 * @param outputs The outputs of the solver including the fluxes.
 */
template <typename MatrixType>
static MatrixType &fill_fluxes(SolverOutputs &outputs) {
    const size_t m = outputs.inputs.diffusionParams.getM();
    const size_t n = outputs.inputs.diffusionParams.getN();
    outputs.fluxes = MyBLAS::Matrix<MyBLAS::NumericType>(m + 2, n + 2, 0);
    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            const size_t idx = (i - 1) * n + (j - 1);
            outputs.fluxes[i][j] = outputs.solution.x[idx];
        }
    }
    return outputs.fluxes;
}

/**
 * @brief Solves a linear system using LUP decomposition.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingLUP(SolverOutputs &outputs, SolverInputs &inputs) {
    inputs.diffusionCoefficients = MyPhysics::Diffusion::Matrix(inputs.diffusionParams);
    auto A = inputs.diffusionCoefficients;
    auto b = naive_fill_diffusion_vector<MyBLAS::NumericType>(inputs);

    if (!MyFactorizationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting LUP calculation\n";
        return;
    }

    auto inMemoryA = MyBLAS::Matrix<MyBLAS::NumericType>(inputs.diffusionCoefficients);

    auto profiler = Profiler([&]() {
        outputs.solution = outputs.solution = MyBLAS::LUP::applyLUP(inMemoryA, b);
    }, inputs.numRuns, inputs.timeout, "LUP");

    outputs.summary = profiler.run().getSummary();
    outputs.summary.runs = profiler.getTotalRuns();
    std::cout<<std::endl<<profiler;

    // post-process
    {
        outputs.solution.converged = true;
        outputs.fluxes = fill_fluxes<MyBLAS::Matrix<MyBLAS::NumericType>>(outputs);
        auto b_prime = A * outputs.solution.x;
        outputs.residual = b - b_prime;
            if (!inputs.fluxOutputDirectory.empty()) {
            writeCSVMatrixNoHeaders(inputs.fluxOutputDirectory, "LUP.csv", outputs.fluxes);
        }
    }
}

/**
 * @brief Solves a linear system using the Point Jacobi method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingPointJacobi(SolverOutputs &outputs, SolverInputs &inputs) {
    inputs.diffusionCoefficients = MyPhysics::Diffusion::Matrix(inputs.diffusionParams);
    auto A = inputs.diffusionCoefficients;
    auto b = naive_fill_diffusion_vector<MyBLAS::NumericType>(inputs);
    const size_t max_iterations = inputs.solverParams.max_iterations;
    const MyBLAS::NumericType threshold = inputs.solverParams.threshold;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting Point jacobi calculation\n";
        return;
    }

    auto profiler = Profiler([&]() {
        outputs.solution = MyRelaxationMethod::applyPointJacobi(A, b, max_iterations, threshold);
    }, inputs.numRuns, inputs.timeout, "Point Jacobi");

    outputs.summary = profiler.run().getSummary();
    outputs.summary.runs = profiler.getTotalRuns();
    std::cout<<std::endl<<profiler;

    // post-process
    {
        outputs.fluxes = fill_fluxes<MyBLAS::Matrix<MyBLAS::NumericType>>(outputs);
        auto b_prime = A * outputs.solution.x;
        outputs.residual = b - b_prime;
        if (!inputs.fluxOutputDirectory.empty()) {
            writeCSVMatrixNoHeaders(inputs.fluxOutputDirectory, "point-jacobi.csv", outputs.fluxes);
        }
    }
}

/**
 * @brief Solves a linear system using the Gauss-Seidel method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingGaussSeidel(SolverOutputs &outputs, SolverInputs &inputs) {
    auto A = MyPhysics::Diffusion::Matrix(inputs.diffusionParams);
    auto b = naive_fill_diffusion_vector<MyBLAS::NumericType>(inputs);
    const size_t max_iterations = inputs.solverParams.max_iterations;
    const MyBLAS::NumericType threshold = inputs.solverParams.threshold;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting Gauss Seidel calculation\n";
        return;
    }

    auto profiler = Profiler([&]() {
        outputs.solution = MyRelaxationMethod::applyGaussSeidel(A, b, max_iterations, threshold);
    }, inputs.numRuns, inputs.timeout, "Gauss Seidel");

    outputs.summary = profiler.run().getSummary();
    outputs.summary.runs = profiler.getTotalRuns();
    std::cout<<std::endl<<profiler;

    // post-process
    {
        outputs.fluxes = fill_fluxes<MyBLAS::Matrix<MyBLAS::NumericType>>(outputs);
        auto b_prime = MatrixVectorExpression<MyPhysics::Diffusion::Matrix, MyBLAS::Vector, MyBLAS::NumericType>::multiplyMatrixVector(A, outputs.solution.x);
        outputs.residual = b - b_prime;
        if (!inputs.fluxOutputDirectory.empty()) {
            writeCSVMatrixNoHeaders(inputs.fluxOutputDirectory, "gauss-seidel.csv", outputs.fluxes);
        }
    }
}

/**
 * @brief Solves a linear system using the Successive Over-Relaxation (SOR) method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingSOR(SolverOutputs &outputs, SolverInputs &inputs) {
    auto A = MyPhysics::Diffusion::Matrix(inputs.diffusionParams);
    auto b = naive_fill_diffusion_vector<MyBLAS::NumericType>(inputs);
    const size_t max_iterations = inputs.solverParams.max_iterations;
    const MyBLAS::NumericType threshold = inputs.solverParams.threshold;
    const MyBLAS::NumericType omega = inputs.solverParams.relaxation_factor;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting SOR calculation\n";
        return;
    }

    auto profiler = Profiler([&]() {
        outputs.solution = MyRelaxationMethod::applySOR(A, b, max_iterations, threshold, omega);
    }, inputs.numRuns, inputs.timeout, "SOR"); // Run the block 10 times

    outputs.summary = profiler.run().getSummary();
    outputs.summary.runs = profiler.getTotalRuns();
    std::cout<<std::endl<<profiler;

    // post-process
    {
        outputs.fluxes = fill_fluxes<MyBLAS::Matrix<MyBLAS::NumericType>>(outputs);
        auto b_prime = MatrixVectorExpression<MyPhysics::Diffusion::Matrix, MyBLAS::Vector, MyBLAS::NumericType>::multiplyMatrixVector(A, outputs.solution.x);
        outputs.residual = b - b_prime;
        if (!inputs.fluxOutputDirectory.empty()) {
            writeCSVMatrixNoHeaders(inputs.fluxOutputDirectory, "SOR.csv", outputs.fluxes);
        }
    }
}

/**
 * @brief Solves a linear system using the Jacobi SOR method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingJacobiSOR(SolverOutputs &outputs, SolverInputs &inputs) {
    auto A = MyPhysics::Diffusion::Matrix(inputs.diffusionParams);
    auto b = naive_fill_diffusion_vector<MyBLAS::NumericType>(inputs);
    const size_t max_iterations = inputs.solverParams.max_iterations;
    const MyBLAS::NumericType threshold = inputs.solverParams.threshold;
    const MyBLAS::NumericType omega = inputs.solverParams.relaxation_factor;
    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting SOR point jacobi calculation\n";
        return;
    }

    auto profiler = Profiler([&]() {
        outputs.solution = MyRelaxationMethod::applyPointJacobi(A, b, max_iterations, threshold, 2.0f - omega);
    }, inputs.numRuns, inputs.timeout, "SORJ");

    outputs.summary = profiler.run().getSummary();
    outputs.summary.runs = profiler.getTotalRuns();
    std::cout<<std::endl<<profiler;

    // post-process
    {
        outputs.fluxes = fill_fluxes<MyBLAS::Matrix<MyBLAS::NumericType>>(outputs);
        auto b_prime = MatrixVectorExpression<MyPhysics::Diffusion::Matrix, MyBLAS::Vector, MyBLAS::NumericType>::multiplyMatrixVector(A, outputs.solution.x);
        outputs.residual = b - b_prime;
        if (!inputs.fluxOutputDirectory.empty()) {
            writeCSVMatrixNoHeaders(inputs.fluxOutputDirectory, "SORJ.csv", outputs.fluxes);
        }
    }
}

/**
 * @brief Solves a linear system using the Symmetric SOR method.
 * @param outputs The output data structure to store the solution and execution time.
 * @param inputs The input data structure containing the system to solve.
 */
void usingSymmetricSOR(SolverOutputs &outputs, SolverInputs &inputs) {
    auto A = MyPhysics::Diffusion::Matrix(inputs.diffusionParams);
    auto b = naive_fill_diffusion_vector<MyBLAS::NumericType>(inputs);
    const size_t max_iterations = inputs.solverParams.max_iterations;
    const MyBLAS::NumericType threshold = inputs.solverParams.threshold;
    const MyBLAS::NumericType omega = inputs.solverParams.relaxation_factor;

    if (!MyRelaxationMethod::passesPreChecks(A, b)) {
        std::cerr << "Aborting symmetric SOR calculation\n";
        return;
    }

    auto profiler = Profiler([&]() {
        outputs.solution = MyRelaxationMethod::applySSOR(A, b, max_iterations, threshold, omega);
    }, inputs.numRuns, inputs.timeout, "SSOR");

    outputs.summary = profiler.run().getSummary();
    outputs.summary.runs = profiler.getTotalRuns();
    std::cout<<std::endl<<profiler;

    // post-process
    {
        outputs.fluxes = fill_fluxes<MyBLAS::Matrix<MyBLAS::NumericType>>(outputs);
        auto b_prime = MatrixVectorExpression<MyPhysics::Diffusion::Matrix, MyBLAS::Vector, MyBLAS::NumericType>::multiplyMatrixVector(A, outputs.solution.x);
        outputs.residual = b - b_prime;
        if (!inputs.fluxOutputDirectory.empty()) {
            writeCSVMatrixNoHeaders(inputs.fluxOutputDirectory, "SSOR.csv", outputs.fluxes);
        }
    }
}
} // namespace Compute

#endif // NE591_008_PROJECT2_COMPUTE_H
