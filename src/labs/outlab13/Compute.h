/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 10/13/2023
 * @brief Compute methods for outlab13 in NE591-008.
 */

#ifndef NE591_008_OUTLAB13_COMPUTE_H
#define NE591_008_OUTLAB13_COMPUTE_H

#include <cmath>
#include <iostream>

#include "FileParser.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/MatrixVectorExpression.h"
#include "math/factorization/LU.h"
#include "math/factorization/LUP.h"
#include "math/relaxation/SOR.h"
#include "math/relaxation/SORPJ.h"
#include "math/relaxation/SSOR.h"

#include <vector>


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

// Function prototype declarations
MyBLAS::NumericType compute_phi_new(size_t i, size_t j, size_t n, MyBLAS::NumericType h, MyBLAS::NumericType rho_0, MyBLAS::NumericType beta, const std::vector<std::vector<MyBLAS::NumericType>>& phi);
bool has_converged(const std::vector<std::vector<MyBLAS::NumericType>>& phi_old, const std::vector<std::vector<MyBLAS::NumericType>>& phi_new, MyBLAS::NumericType &tolerance);

/**
 * @brief Checks if the iteration has converged based on the maximum residual.
 * 
 * @param phi Scalar flux distribution from the current iteration.
 * @param n Number of mesh points in each direction, excluding the boundary points.
 * @param h Mesh spacing.
 * @param rho_0 Base removal term.
 * @param beta Coefficient for the nonlinearity in the removal term.
 * @param tolerance Convergence tolerance.
 * @return True if converged, False otherwise.
 */
bool has_converged_residual(const std::vector<std::vector<MyBLAS::NumericType>>& phi, size_t n, MyBLAS::NumericType h, MyBLAS::NumericType rho_0, MyBLAS::NumericType beta, MyBLAS::NumericType &tolerance) {
    MyBLAS::NumericType max_residual = 0.0;
    MyBLAS::NumericType h2 = h * h;
    
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            MyBLAS::NumericType rho = rho_0;
            if (phi[i][j] > 0) {
                rho += beta / std::sqrt(phi[i][j] + 1e-6);
            }
            MyBLAS::NumericType residual = std::abs(
                (-phi[i+1][j] + 2.0 * phi[i][j] - phi[i-1][j]) / h2 +
                (-phi[i][j+1] + 2.0 * phi[i][j] - phi[i][j-1]) / h2 +
                rho * phi[i][j] - 1.0
            );
            max_residual = std::max(max_residual, residual);
        }
    }

    auto convered = (max_residual < tolerance);
    tolerance = max_residual;
    return convered;
}

/**
 * @brief Solves the nonlinear neutron diffusion equation using Fixed Point Iterations.
 *
 * @param n Number of mesh points in each direction, excluding the boundary points.
 * @param L Side length of the square problem domain.
 * @param rho_0 Base removal term.
 * @param beta Coefficient for the nonlinearity in the removal term.
 * @param max_iterations Maximum number of iterations to perform.
 * @param tolerance Convergence tolerance based on the infinity norm of the difference between iterations.
 * @return The final scalar flux distribution phi.
 */
MyBLAS::Solver::Solution<MyBLAS::NumericType> solve_diffusion_equation(size_t n, MyBLAS::NumericType L, MyBLAS::NumericType rho_0, MyBLAS::NumericType beta, size_t max_iterations, MyBLAS::NumericType tolerance) {

    // Calculate the mesh spacing
    MyBLAS::NumericType h = L / (static_cast<MyBLAS::NumericType>(n) + 1);

    // Initialize the scalar flux phi with an initial guess (e.g., zeros)
    std::vector<std::vector<MyBLAS::NumericType>> phi(n + 2, std::vector<MyBLAS::NumericType>(n + 2, 0.0));
    std::vector<std::vector<MyBLAS::NumericType>> phi_new(n + 2, std::vector<MyBLAS::NumericType>(n + 2, 0.0));

    // Perform Fixed Point Iterations
    size_t iteration;
    for (iteration = 0; iteration < max_iterations; iteration++) {
        // Update phi values for the interior points
        for (size_t i = 1; i <= n; ++i) {
            for (size_t j = 1; j <= n; ++j) {
                phi_new[i][j] = compute_phi_new(i, j, n, h, rho_0, beta, phi);
            }
        }

        // Check for convergence
//        if (has_converged(phi, phi_new, tolerance)) {
        if (has_converged_residual(phi_new, n, h, rho_0, beta, tolerance)) {
            //std::cout << "Convergence achieved after " << iteration + 1 << " iterations." << std::endl;
            break;
        }

        // Update phi for the next iteration
        phi = phi_new;
    }

    MyBLAS::Solver::Solution<MyBLAS::NumericType> results(n+2);
    results.phi.getData() = phi;
    results.converged = (iteration < max_iterations);
    results.iterations = iteration + 1;
    results.iterative_error = tolerance;
    return results;
}

/**
 * @brief Computes the new scalar flux value for node (i, j) using the current iteration's values.
 *
 * @param i Row index of the current node.
 * @param j Column index of the current node.
 * @param n Number of mesh points in each direction, excluding the boundary points.
 * @param h Mesh spacing.
 * @param rho_0 Base removal term.
 * @param beta Coefficient for the nonlinearity in the removal term.
 * @param phi Current scalar flux distribution.
 * @return The new scalar flux value for node (i, j).
 */
MyBLAS::NumericType compute_phi_new(size_t i, size_t j, size_t n, MyBLAS::NumericType h, MyBLAS::NumericType rho_0, MyBLAS::NumericType beta, const std::vector<std::vector<MyBLAS::NumericType>>& phi) {
    MyBLAS::NumericType h2 = h * h;
    MyBLAS::NumericType phi_ij = phi[i][j];

    // Compute the nonlinear term rho
    MyBLAS::NumericType rho = rho_0;
    if (phi_ij > 0) {
        rho += (beta / std::sqrt(phi_ij + 1e-8));
    }

    // Apply the discretized equation to calculate the new flux value
//    MyBLAS::NumericType phi_new_ij2 = (phi[i+1][j] + phi[i-1][j] + phi[i][j+1] + phi[i][j-1] - h2) / (2.0 * h2 + rho * h2);
    // Apply the discretized equation to calculate the new flux value
    MyBLAS::NumericType phi_new_ij = (phi[i+1][j] + phi[i-1][j] + phi[i][j+1] + phi[i][j-1] + h2) / (2.0 * h2 + rho * h2); // Corrected the source term sign
    return phi_new_ij;
}

/**
 * @brief Checks if the iteration has converged based on the infinity norm of the difference.
 *
 * @param phi_old Scalar flux distribution from the previous iteration.
 * @param phi_new Scalar flux distribution from the current iteration.
 * @param tolerance Convergence tolerance.
 * @return True if converged, False otherwise.
 */
bool has_converged(const std::vector<std::vector<MyBLAS::NumericType>>& phi_old, const std::vector<std::vector<MyBLAS::NumericType>>& phi_new, MyBLAS::NumericType &tolerance) {
    MyBLAS::NumericType max_diff = 0.0;
    for (size_t i = 1; i < phi_old.size() - 1; ++i) {
        for (size_t j = 1; j < phi_old[i].size() - 1; ++j) {
            MyBLAS::NumericType diff = std::abs(phi_new[i][j] - phi_old[i][j]);
            max_diff = std::max(max_diff, diff);
        }
    }
    const bool converged = (max_diff < tolerance);
    tolerance = max_diff;
    return converged;
}

void usingFixedPoint(SolverOutputs &outputs, SolverInputs &inputs) {

    // Define problem parameters
    auto n = inputs.n; // Number of mesh points in each direction, excluding the boundary points
    auto L = inputs.L; // Side length of the square problem domain
    auto rho_0 = inputs.rho; // Base removal term
    auto beta = inputs.beta; // Coefficient for the nonlinearity in the removal term
    auto max_iterations = inputs.solverParams.max_iterations; // Maximum number of iterations to perform
    auto tolerance = inputs.solverParams.convergence_threshold; // Convergence tolerance

    auto profiler = Profiler([&]() {
        // Solve the nonlinear neutron diffusion equation
        outputs.solution = solve_diffusion_equation(n, L, rho_0, beta, max_iterations, tolerance);
    }, 2, 0, "Fixed Point Method").run();

    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;


    // Output the final scalar flux distribution
    std::cout << "Final scalar flux distribution:" << std::endl;
    MyBLAS::NumericType sum = 0;
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            sum += outputs.solution.phi[i][j];
            std::cout << std::fixed << std::setprecision(6) << outputs.solution.phi[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout<<"Average Flux: "<<sum/std::pow(n, 2)<<std::endl;
}

void usingNewtons(SolverOutputs &outputs, SolverInputs &inputs) {

    // Define problem parameters
    auto n = inputs.n; // Number of mesh points in each direction, excluding the boundary points
    auto L = inputs.L; // Side length of the square problem domain
    auto rho_0 = inputs.rho; // Base removal term
    auto beta = inputs.beta; // Coefficient for the nonlinearity in the removal term
    auto max_iterations = inputs.solverParams.max_iterations; // Maximum number of iterations to perform
    auto tolerance = inputs.solverParams.convergence_threshold; // Convergence tolerance

    auto profiler = Profiler([&]() {
                        // Solve the nonlinear neutron diffusion equation
                        outputs.solution = solve_diffusion_equation(n, L, rho_0, beta, max_iterations, tolerance);
                    }, 2, 0, "Netwon's Method").run();

    outputs.summary = profiler.getSummary();
    std::cout << profiler << std::endl;
    std::cout << outputs.solution <<std::endl;

    // Output the final scalar flux distribution
    std::cout << "Final scalar flux distribution:" << std::endl;
    MyBLAS::NumericType sum = 0;
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            sum += outputs.solution.phi[i][j];
            std::cout << std::fixed << std::setprecision(6) << outputs.solution.phi[i][j];
        }
        std::cout << std::endl;
    }
    std::cout<<"Average Flux: "<<sum/std::pow(n, 2)<<std::endl;
}
} // namespace Compute

#endif // NE591_008_OUTLAB13_COMPUTE_H
