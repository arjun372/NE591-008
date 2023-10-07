/**
 * @file Circuit.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief Header file for Circuit function, which generates a coefficient matrix A, and vectors x and b for a circuit.
*/

#ifndef NE591_008_CIRCUIT_H
#define NE591_008_CIRCUIT_H

#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include "math/Random.h"

/**
 * @namespace MyBLAS::System
 * @brief Namespace for system-related functions in MyBLAS library.
 */
namespace MyBLAS::System {

    /**
     * @brief Generates a circuit with random resistances and currents, and computes the corresponding voltages.
     * The circuit is represented by a system of linear equations Ax = b, where A is the resistance matrix,
     * x is the current vector, and b is the voltage vector.
     *
     * @tparam T The data type of the elements in the circuit (e.g., double for real-valued resistances, currents, and voltages).
     * @param n The size of the circuit (i.e., the number of resistances/currents/voltages).
     * @param[out] A The resistance matrix of the circuit, which is made to be (barely) diagonally dominant.
     * @param[out] b The voltage vector of the circuit, computed as the product of the resistance matrix and the current vector.
     * @param[out] x The current vector of the circuit, randomly generated.
     * @param seed The seed for the random number generator (default is 372).
     *
     * @note The resistances are randomly generated in the range [1, 10e3] Ω.
     * The currents are randomly generated in the range [-5, 5] Amperes.
     * The function will print a warning message to the standard error stream if it cannot make the resistance matrix diagonally dominant.
     */
    template<typename T>
    void Circuit(const size_t n, MyBLAS::Matrix<T> &A, MyBLAS::Vector<T> &b, MyBLAS::Vector<T> &x, const size_t seed = 372) {

        // 1Ω to 10 kΩ
        const T min_resistance = 1;
        const T max_resistance = 10e3;

        // 5 Amps in either direction
        const T min_current = -5;
        const T max_current =  5;

        // Create the coefficient matrix A, and vectors x and b
        MyBLAS::Vector<T> currents = Random::generate(n, static_cast<T>(min_current), static_cast<T>(max_current), seed);

        // random field of resistors, each with a resistance sampled from U(1, 10k) Ω
        MyBLAS::Matrix<T> resistors = Random::binary<T>(n, n, seed);
        MyBLAS::Matrix<T> resistances = Random::generate(n, n, static_cast<T>(min_resistance), static_cast<T>(max_resistance), seed);
        resistances = MyBLAS::innerProduct(resistances, resistors);

        // make (barely) diagonally dominant
        const T dominance = 1;
        MyBLAS::makeDiagonallyDominant(resistances, dominance);

        if(!isDiagonallyDominant(resistances)) {
            std::cerr<<"Warning: Could not make resistance matrix diagonally dominant\n";
        }

        MyBLAS::Vector<T> voltages = resistances * currents;

        A = resistances;
        b = voltages;
        x = currents;
    }
}

#endif //NE591_008_CIRCUIT_H
