/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/29/2023
 * @brief Compute methods for project milestone 1 in NE591-008.
 */

#ifndef NE591_008_PROJECT1_COMPUTE_H
#define NE591_008_PROJECT1_COMPUTE_H

#include <iostream>
#include <cmath>

#include "math/blas/Matrix.h"
#include "math/blas/LU.h"
#include "math/blas/LUP.h"

/**
 * @brief Calculates the mesh spacings in the x and y directions.
 *
 * This function computes the mesh spacings delta and gamma using the given
 * formulas: delta = a / (m + 1) and gamma = b / (n + 1). These spacings are
 * used to discretize the diffusion operator in the given equation.
 *
 * @struct SolverInputs inputs
 * @param inputs.a The length of the rectangular region in the x-direction.
 * @param inputs.b The length of the rectangular region in the y-direction.
 * @param inputs.m The number of mesh points in the x-direction, excluding the boundary points.
 * @param inputs.n The number of mesh points in the y-direction, excluding the boundary points.
 * @param[out] inputs.delta The mesh spacing in the x-direction.
 * @param[out] inputs.gamma The mesh spacing in the y-direction.
 */
void calculate_mesh_spacings(SolverInputs &inputs)
{
    // Compute the mesh spacing in the x-direction using the formula: delta = a / (m + 1)
    inputs.delta = inputs.a / (inputs.m + 1);

    // Compute the mesh spacing in the y-direction using the formula: gamma = b / (n + 1)
    inputs.gamma = inputs.b / (inputs.n + 1);
}

/**
 * @brief Initializes the diffusion matrix and vector.
 *
 * This function initializes the diffusion matrix A and the right-hand-side vector B.
 * The matrix and vector are resized to m x n and all elements are initialized to zero.
 *
 * @tparam T The type of the elements in the matrix and vector.
 * @param m The number of rows in the matrix and vector.
 * @param n The number of columns in the matrix and vector.
 * @return IntermediateResults The initialized diffusion matrix A and right-hand-side vector B.
 */
template <typename T>
static IntermediateResults initialize_diffusion_matrix_and_vector(size_t m, size_t n) {

    IntermediateResults intermediates;

    // Resize the matrix A to m x n and initialize all elements to zero
    intermediates.diffusion_matrix_A = MyBLAS::Matrix<T>(std::vector<std::vector<T>>(m * n, std::vector<T>(m * n, 0)));

    // Resize the right-hand-side vector B to m x n and initialize all elements to zero
    intermediates.right_hand_side_vector_B = MyBLAS::Vector<T>(std::vector<T>(m * n, 0));

    return intermediates;
}

/**
 * @brief Fills the diffusion matrix and vector.
 *
 * This function fills the diffusion matrix A and the right-hand-side vector B based on the given inputs.
 * The diagonal and off-diagonal elements of the matrix A are calculated using the given equations.
 * The right-hand-side vector B is filled using the fixed source q(i, j).
 *
 * @param inputs The inputs for the solver.
 * @param intermediates The intermediate results including the diffusion matrix A and right-hand-side vector B.
 */
static void naive_fill_diffusion_matrix_and_vector(SolverInputs &inputs, IntermediateResults &intermediates) {

    const size_t m = inputs.m;
    const size_t n = inputs.n;

    const long double D = inputs.diffusion_coefficient;
    const long double D_over_delta_squared = D / (inputs.delta * inputs.delta);
    const long double minus_D_over_delta_squared = -D_over_delta_squared;
    const long double D_over_gamma_squared = D / (inputs.gamma * inputs.gamma);
    const long double minus_D_over_gamma_squared = -D_over_gamma_squared;
    const long double cross_section = inputs.macroscopic_removal_cross_section;
    const long double diagonal = 2.0 * (D_over_delta_squared + D_over_gamma_squared) + cross_section;

    // Loop through all the nodes i = 1, ..., m and j = 1, ..., n
    for (size_t i = 1; i <= m; ++i)
    {
        for (size_t j = 1; j <= n; ++j)
        {
            // Calculate the index of the current node in the matrix A and vector B
            int idx = (i - 1) * n + (j - 1);

            // Fill the diagonal element of the matrix A using the given equation
            intermediates.diffusion_matrix_A[idx][idx] = diagonal;

            // Fill the off-diagonal elements of the matrix A using the given equation
            if (i > 1)
            {
                intermediates.diffusion_matrix_A[idx][idx - n] = minus_D_over_delta_squared;
            }
            if (i < m)
            {
                intermediates.diffusion_matrix_A[idx][idx + n] =  minus_D_over_delta_squared;
            }
            if (j > 1)
            {
                intermediates.diffusion_matrix_A[idx][idx - 1] =  minus_D_over_gamma_squared;
            }
            if (j < n)
            {
                intermediates.diffusion_matrix_A[idx][idx + 1] = minus_D_over_gamma_squared;
            }

            // Fill the right-hand-side vector B using the fixed source q(i, j)
            intermediates.right_hand_side_vector_B[idx] = inputs.sources[i - 1][j - 1];
        }
    }
}

/**
 * @brief Solves the linear system.
 *
 * This function solves the linear system by factorizing the diffusion matrix A into L, U, and P matrices.
 * It also checks if the factorized matrices L and U are unit lower triangular and upper triangular respectively,
 * and if the generated matrix P is a permutation matrix.
 *
 * @param intermediates The intermediate results including the diffusion matrix A and the factorized matrices L, U, and P.
 */
static void naive_solve_linear_system(IntermediateResults &intermediates) {

    const auto rows = intermediates.diffusion_matrix_A.getRows();
    const auto cols = intermediates.diffusion_matrix_A.getCols();

    intermediates.L = MyBLAS::Matrix(rows, cols, static_cast<long double>(0));
    intermediates.U = MyBLAS::Matrix(rows, cols, static_cast<long double>(0));
    intermediates.P = MyBLAS::LUP::factorize(intermediates.L, intermediates.U, intermediates.diffusion_matrix_A);

    if(!MyBLAS::isUnitLowerTriangularMatrix(intermediates.L)) {
        std::cerr << "Warning: Factorized matrix L is not unit lower triangular, expect undefined behavior.\n";
    }

    if(!MyBLAS::isUpperTriangularMatrix(intermediates.U)) {
        std::cerr << "Warning: Factorized matrix U is not upper triangular, expect undefined behavior.\n";
    }

    if (!MyBLAS::isPermutationMatrix(intermediates.P)) {
        std::cerr << "Warning: Generated matrix P is not a permutation matrix, expect undefined behavior.\n";
    }
}

/**
 * @brief Fills the fluxes.
 *
 * This function fills the fluxes based on the given phi vector.
 * The fluxes are stored in a matrix with m+2 rows and n+2 columns.
 *
 * @param phi The phi vector.
 * @param inputs The inputs for the solver.
 * @param outputs The outputs of the solver including the fluxes.
 */
static void fill_fluxes(MyBLAS::Vector<long double> phi, SolverInputs &inputs, SolverOutputs &outputs) {
    const size_t m = inputs.m;
    const size_t n = inputs.n;

    outputs.fluxes = MyBLAS::Matrix<long double>(m+2, n+2, 0);

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            const size_t idx = (i - 1) * n + (j - 1);
            outputs.fluxes[i][j] = phi[idx];
        }
    }
}

#endif //NE591_008_PROJECT1_COMPUTE_H
