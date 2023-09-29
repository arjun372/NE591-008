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
 * @brief Initializes the matrix A and the right-hand-side vector B with zeros.
 *
 * This function creates an m x n matrix A and an m x n right-hand-side vector B,
 * initializing all elements to zero.
 *
 * @param m The number of mesh points in the x-direction, excluding the boundary points.
 * @param n The number of mesh points in the y-direction, excluding the boundary points.
 * @param[out] A The m x n matrix A initialized with zeros.
 * @param[out] B The m x n right-hand-side vector B initialized with zeros.
 */
void initialize_matrix_and_vector(int m, int n, std::vector<std::vector<double>>& A, std::vector<double>& B)
{
    // Resize the matrix A to m x n and initialize all elements to zero
    A.resize(m * n, std::vector<double>(m * n, 0.0));

    // Resize the right-hand-side vector B to m x n and initialize all elements to zero
    B.resize(m * n, 0.0);
}

/**
 * @brief Fills the matrix A and the right-hand-side vector B using the given equation and input parameters.
 *
 * This function loops through all the nodes i = 1, ..., m and j = 1, ..., n, and fills the matrix A and
 * the right-hand-side vector B using the given equation and the input parameters.
 *
 * @param m The number of mesh points in the x-direction, excluding the boundary points.
 * @param n The number of mesh points in the y-direction, excluding the boundary points.
 * @param D The constant diffusion coefficient.
 * @param Sigma_a The constant macroscopic removal cross-section.
 * @param delta The mesh spacing in the x-direction.
 * @param gamma The mesh spacing in the y-direction.
 * @param q The non-uniformly distributed fixed source at node i, j.
 * @param[out] A The filled m x n matrix A.
 * @param[out] B The filled m x n right-hand-side vector B.
 */
void fill_matrix_and_vector(int m, int n, double D, double Sigma_a, double delta, double gamma, const std::vector<std::vector<double>>& q, std::vector<std::vector<double>>& A, std::vector<double>& B)
{
    // Loop through all the nodes i = 1, ..., m and j = 1, ..., n
    for (int i = 1; i <= m; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            // Calculate the index of the current node in the matrix A and vector B
            int idx = (i - 1) * n + (j - 1);

            // Fill the diagonal element of the matrix A using the given equation
            A[idx][idx] = 2 * D * (1 / (delta * delta) + 1 / (gamma * gamma)) + Sigma_a;

            // Fill the off-diagonal elements of the matrix A using the given equation
            if (i > 1)
            {
                A[idx][idx - n] = -D / (delta * delta);
            }
            if (i < m)
            {
                A[idx][idx + n] = -D / (delta * delta);
            }
            if (j > 1)
            {
                A[idx][idx - 1] = -D / (gamma * gamma);
            }
            if (j < n)
            {
                A[idx][idx + 1] = -D / (gamma * gamma);
            }

            // Fill the right-hand-side vector B using the fixed source q(i, j)
            B[idx] = q[i - 1][j - 1];
        }
    }
}

/**
 * @brief Solves the linear system A * phi = B using LU factorization with pivoting.
 *
 * This function solves the linear system A * phi = B using LU factorization with pivoting
 * and stores the solution in the vector phi.
 *
 * @param A The m x n matrix A.
 * @param B The m x n right-hand-side vector B.
 * @param[out] phi The solution vector containing the scalar fluxes at each node i, j.
 */
void solve_linear_system(const std::vector<std::vector<double>>& A, const std::vector<double>& B, std::vector<double>& phi)
{
    // Use Eigen library to solve the linear system A * phi = B using LU factorization with pivoting
//    Eigen::MatrixXd A_eigen(A.size(), A[0].size());
//    Eigen::VectorXd B_eigen(B.size());
//    Eigen::VectorXd phi_eigen;
//
//    for (int i = 0; i < A.size(); ++i)
//    {
//        for (int j = 0; j < A[0].size(); ++j)
//        {
//            A_eigen(i, j) = A[i][j];
//        }
//        B_eigen(i) = B[i];
//    }
//
//    phi_eigen = A_eigen.partialPivLu().solve(B_eigen);
//
//    // Copy the solution from the Eigen vector to the output vector phi
//    phi.resize(phi_eigen.size());
//    for (int i = 0; i < phi_eigen.size(); ++i)
//    {
//        phi[i] = phi_eigen(i);
//    }
}

/**
 * @brief Writes the scalar fluxes phi(i, j) to an output file.
 *
 * This function writes the scalar fluxes phi(i, j) for i = 1, ..., m and j = 1, ..., n
 * to an output file.
 *
 * @param output_file The name of the output file.
 * @param m The number of mesh points in the x-direction, excluding the boundary points.
 * @param n The number of mesh points in the y-direction, excluding the boundary points.
 * @param phi The solution vector containing the scalar fluxes at each node i, j.
 */
void write_output(std::string &output_file, int m, int n, const std::vector<double>& phi)
{
    std::ofstream out(output_file);

    if (out.is_open())
    {
        // Write the scalar fluxes phi(i, j) to the output file
        for (int i = 1; i <= m; ++i)
        {
            for (int j = 1; j <= n; ++j)
            {
                int idx = (i - 1) * n + (j - 1);
                out << "phi(" << i << ", " << j << ") = " << phi[idx] << std::endl;
            }
        }

        out.close();
    }
    else
    {
        std::cerr << "Unable to open output file: " << output_file << std::endl;
    }
}

#endif //NE591_008_PROJECT1_COMPUTE_H
