/**
 * @file Circuit.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief Header file for the Circuit function, which generates a coefficient matrix A, and vectors x, b for a circuit.
 */

#ifndef NE591_008_CIRCUIT_H
#define NE591_008_CIRCUIT_H

#include "math/Random.h"
#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

/**
* @namespace MyBLAS::System
* @brief This namespace contains system-related functions in the MyBLAS library.
*/
namespace MyBLAS::System {

/**
* @brief This function generates a circuit with random resistances and currents, and computes the corresponding voltages.
* The circuit is represented by a system of linear equations Ax = b, where A is the resistance matrix,
* x is the current vector, and b is the voltage vector.
*
* @tparam T The data type of the elements in the circuit (e.g., double for real-valued resistances, currents, and
* voltages).
* @param n The size of the circuit (i.e., the number of resistances/currents/voltages).
* @param[out] A The resistance matrix of the circuit, which is made to be (barely) diagonally dominant.
* @param[out] b The voltage vector of the circuit, computed as the product of the resistance matrix and the current
* vector.
* @param[out] x The current vector of the circuit, randomly generated.
* @param seed The seed for the random number generator (default is 372).
*
* @note The resistances are randomly generated in the range [1, 10e3] Ω.
* The currents are randomly generated in the range [-5, 5] Amperes.
* The function will print a warning message to the standard error stream if it cannot make the resistance matrix
* diagonally dominant.
*/
template <typename T>
void Circuit(const size_t n, MyBLAS::Matrix<T> &A, MyBLAS::Vector<T> &b, MyBLAS::Vector<T> &x, const size_t seed = 372)
{

   // Define the range of resistance values from 1Ω to 10 kΩ
   const T min_resistance = 1;
   const T max_resistance = 10e3;

   // Define the range of current values from -5 Amps to 5 Amps
   const T min_current = -5;
   const T max_current = 5;

   // Generate a vector of random currents within the defined range
   MyBLAS::Vector<T> currents = Random::generate(n, min_current, max_current, seed);

   // Generate a matrix of random resistors, each with a resistance sampled from U(1, 10k) Ω
   // Start with a 2D bitmap of resistors
   MyBLAS::Matrix<T> resistors = Random::binary<T>(n, n, seed);

   // Then, sample resistances from the distribution
   MyBLAS::Matrix<T> resistances = Random::generate(n, n, min_resistance, max_resistance, seed);

   // Finally, element-wise multiply these with the bitmap, assigning the resistors their magnitudes
   // @note: negative resistances are OK here as long as the currents in the previous step are also randomly sampled
   // In general, when applying Kirchoff's rules, direction of current flow is not know apriori.
   resistances = MyBLAS::innerProduct(resistances, resistors);

   // Make the resistance matrix (barely) diagonally dominant
   // We just increase the dominance by +1 integer value. It doesn't matter what the magnitude is, as long as it is
   // applied as a scalar offset. If it was applied as a multiplier or some other function, then diagonal dominance will
   // become a function of the matrix size, sampling distribution, and sampling range. This is not good for our
   // benchmarks since we need the extent of dominance to be invariant. Otherwise, matrix size will affect convergence.
   const T dominance = 1;
   MyBLAS::makeDiagonallyDominant(resistances, dominance);

   // If the resistance matrix is not diagonally dominant, print a warning message
   if (!isDiagonallyDominant(resistances)) {
       std::cerr << "Warning: Could not make resistance matrix diagonally dominant\n";
   }

   // Compute the voltage vector as the product of the resistance matrix and the current vector
   MyBLAS::Vector<T> voltages = resistances * currents;

   // Assign the computed values to the output parameters
   A = resistances;
   b = voltages;
   x = currents;
}
} // namespace MyBLAS::System

#endif // NE591_008_CIRCUIT_H
