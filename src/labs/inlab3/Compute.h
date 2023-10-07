/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/08/2023
 * @brief Compute methods for inlab3 in NE591-008.
 */

#pragma once

#include <cmath>
#include <iostream>

#include "CommandLine.h"
#include "InputsOutputs.h"
#include "Stopwatch.h"

/**
 * @brief Computes the Lagrange polynomial for a given interpolation point xi.
 * @tparam T The data type of the elements in the vectors.
 * @param xi The interpolation point.
 * @param x A vector containing the x-coordinates of the data points.
 * @param fx A vector containing the y-coordinates (function values) of the data points.
 * @return The value of the Lagrange polynomial at the interpolation point xi.
 */
template <typename T> static T getLagrangePoly(const T xi, const std::vector<T> &x, const std::vector<T> &fx) {
    T Px = 0.0f;
    const size_t n = std::min(x.size(), fx.size());
    for (size_t j = 0; j < n; j++) {
        T Pjx = fx[j];

        // go from k -> j-1
        T Pjx_product1 = 1.0f;
        for (size_t k = 0; k < j; k++) {
            T Pjxk = (xi - x[k]) / (x[j] - x[k]);
            Pjx_product1 *= Pjxk;
        }

        // skip k == j;

        // then, from k+1 -> n
        T Pjx_product2 = 1.0f;
        for (size_t k = j + 1; k < n; k++) {
            T Pjxk = (xi - x[k]) / (x[j] - x[k]);
            Pjx_product2 *= Pjxk;
        }
        Px += (Pjx * Pjx_product1 * Pjx_product2);
    }
    return Px;
}
/**
 * @brief This function computes the Lagrange polynomials for a given set of data points.
 * @tparam T The data type of the elements in the vectors.
 * @param Lxi A vector to store the computed Lagrange polynomials.
 * @param xi A vector containing the x-coordinates of the interpolation points.
 * @param x A vector containing the x-coordinates of the data points.
 * @param fx A vector containing the y-coordinates (function values) of the data points.
 * @note The function also prints the time taken to compute the Lagrange polynomials.
 */
template <typename T>
static void fillLagrangePolys(std::vector<T> &Lxi, const std::vector<T> &xi, const std::vector<T> &x,
                              const std::vector<T> &fx) {
    const size_t m = std::min(xi.size(), Lxi.size());
    Lxi.reserve(m);

    for (size_t i = 0; i < m; i++) {
        Lxi[i] = getLagrangePoly(xi[i], x, fx);
    }
}

/**
 * @brief This function computes the interpolation error for a given set of data points and their corresponding Lagrange
 * polynomials.
 * @tparam T The data type of the elements in the vectors.
 * @param IEx A vector to store the computed interpolation errors.
 * @param Lxi A vector containing the computed Lagrange polynomials.
 * @param fxi A vector containing the function values at the interpolation points.
 * @note The function will print an error message and return if the sizes of Lxi and fxi are not equal.
 */
template <typename T>
static void fillInterpolationError(std::vector<T> &IEx, const std::vector<T> &Lxi, const std::vector<T> &fxi) {
    // Check if the input vectors have the same size
    if (Lxi.size() != fxi.size()) {
        std::cerr << "Error: L(xi), f(xi) must have the same size." << std::endl;
        return;
    }

    IEx.clear();
    IEx.resize(Lxi.size());

    // Use std::transform with a lambda function to calculate absolute errors
    std::transform(Lxi.begin(), Lxi.end(), fxi.begin(), IEx.begin(), [](T x, T y) { return std::abs(x - y); });
}

/**
 * @brief This function computes the integral of a user-defined function using the trapezoidal rule.
 * @param outputs A reference to a NewtonCotesOutputs object to store the computed integral and step size.
 * @param inputs A constant reference to a NewtonCotesInputs object containing the input parameters for the trapezoidal
 * rule.
 * @return The computed integral value as a long double.
 * @note The function also prints the time taken to compute the integral.
 */
static long double fillUsingTrapezoidal(NewtonCotesOutputs &outputs, const NewtonCotesInputs &inputs) {

    // setup
    const size_t m = inputs.m;
    const long double b = inputs.b;
    const long double a = inputs.a;
    const long double h = (b - a) / m;

    Stopwatch<Nanoseconds> timer;
    timer.restart();

    // long double h = (b - a) / (n);
    long double compositeIntegral = user_defined_fx(a) + user_defined_fx(b);

    for (size_t i = 1; i < m; i++) {
        compositeIntegral += 2 * user_defined_fx(a + i * h);
    }

    timer.click();
    std::cout << "Computing integral using trapezoidal rule for m=(" << m << ") intervals took "
              << (static_cast<long double>(timer.duration().count())) << " ns" << std::endl;

    // finally, update the output values
    outputs.h = h;
    outputs.integral = (h / 2.0) * compositeIntegral;

    if (inputs.flip_integral) {
        outputs.integral = -outputs.integral;
    }

    return outputs.integral;
}

/**
 * @brief This function computes the integral of a user-defined function using Simpson's rule.
 * @param outputs A reference to a NewtonCotesOutputs object to store the computed integral and step size.
 * @param inputs A constant reference to a NewtonCotesInputs object containing the input parameters for Simpson's rule.
 * @return The computed integral value as a long double.
 * @note The function also prints the time taken to compute the integral.
 */
static long double fillUsingSimpsons(NewtonCotesOutputs &outputs, const NewtonCotesInputs &inputs) {

    const size_t m = inputs.m;
    const long double b = inputs.b;
    const long double a = inputs.a;
    const long double h = (b - a) / m;

    Stopwatch<Nanoseconds> timer;
    timer.restart();

    long double sum = user_defined_fx(a) + user_defined_fx(b);

    for (size_t i = 1; i < m; i += 2) {
        sum += 4.0f * user_defined_fx(a + i * h);
    }

    for (size_t i = 2; i < m - 1; i += 2) {
        sum += 2.0f * user_defined_fx(a + i * h);
    }

    timer.click();
    std::cout << "Computing integral using Simpson's rule for m=(" << m << ") intervals took "
              << (static_cast<long double>(timer.duration().count())) << " ns" << std::endl;

    outputs.h = h;
    outputs.integral = h * sum / 3.0f;

    if (inputs.flip_integral) {
        outputs.integral = -outputs.integral;
    }

    return outputs.integral;
}

/**
 * @brief This function computes the integral of a user-defined function using Gaussian quadrature.
 * @param outputs A reference to a NewtonCotesOutputs object to store the computed integral and step size.
 * @param inputs A constant reference to a NewtonCotesInputs object containing the input parameters for Gaussian
 * quadrature.
 * @return The computed integral value as a long double.
 * @note The function is not implemented yet and will print a message indicating that it is not available.
 */
static long double fillUsingGaussianQuadratures(NewtonCotesOutputs &outputs, const NewtonCotesInputs &inputs) {
    std::cout << "\tGauss-Legendre Quadrature not available yet." << std::endl;

    if (inputs.flip_integral) {
        outputs.integral = -outputs.integral;
    }

    return outputs.integral;
}
