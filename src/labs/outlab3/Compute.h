/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/15/2023
 * @brief Compute methods for OutLab3 in NE591-008.
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>

#include "InputsOutputs.h"

#include "CommandLine.h"
#include "extern/function.h"
#include "math/RootFinder.h"
#include "utils/profiler/Stopwatch.h"

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

    long double totalTime = 0.0f;
    for (size_t runs = 0; runs < 1000; runs++) {

        Stopwatch<Nanoseconds> timer;
        timer.restart();

        // long double h = (b - a) / (n);
        long double compositeIntegral = user_defined_fx(a) + user_defined_fx(b);

        for (size_t i = 1; i < m; i++) {
            compositeIntegral += 2.0f * user_defined_fx(a + i * h);
        }

        const long double integral = (h / 2.0f) * compositeIntegral;

        timer.click();

        // finally, update the output values
        outputs.h = h;
        outputs.integral = integral;
        totalTime += static_cast<long double>(timer.duration().count());
    }

    std::cout << "Computing 1000 integrals using trapezoidal rule for m=(" << m << ") intervals took " << (totalTime)
              << " ns, avg: " << (totalTime) / 1000.f << std::endl;

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

    long double totalTime = 0.0f;
    for (size_t runs = 0; runs < 1000; runs++) {

        Stopwatch<Nanoseconds> timer;
        timer.restart();

        long double sum = user_defined_fx(a) + user_defined_fx(b);

        for (size_t i = 1; i < m; i += 2) {
            sum += 4.0f * user_defined_fx(a + i * h);
        }

        for (size_t i = 2; i < m - 1; i += 2) {
            sum += 2.0f * user_defined_fx(a + i * h);
        }

        const long double integral = h * sum / 3.0f;

        timer.click();

        outputs.h = h;
        outputs.integral = integral;
        totalTime += static_cast<long double>(timer.duration().count());
    }

    std::cout << "Computing 1000 integrals using Simpson's rule for m=(" << m << ") intervals took " << (totalTime)
              << " ns, avg: " << (totalTime) / 1000.f << std::endl;

    if (inputs.flip_integral) {
        outputs.integral = -outputs.integral;
    }

    return outputs.integral;
}

/**
 * @brief Computes the value of the Legendre polynomial of degree n at a given point.
 *
 * This function uses a recursive approach to compute the value of the Legendre polynomial
 * of degree n at a given point.
 *
 * @tparam T The data type of the input point and the output value.
 * @param n The degree of the Legendre polynomial. This should be a non-negative integer.
 * @param point The point at which the Legendre polynomial is to be evaluated.
 * @return The value of the Legendre polynomial of degree n at the given point.
 *
 * @note The function uses a recursive approach, which may not be the most efficient method
 *       for large values of n. For better performance, consider using an iterative approach
 *       or a precomputed table of Legendre polynomial values.
 */
template <typename T> static T getLegendrePolynomial(const size_t n, const T point) {
    if (n <= 0) {
        return 1.0f;
    }
    if (n == 1) {
        return point;
    }
    const T P1 = getLegendrePolynomial(n - 1, point);
    const T P0 = getLegendrePolynomial(n - 2, point);
    const T Pnx1 = ((2 * n) - 1) * point * P1;
    const T Pnx0 = (n - 1) * P0;
    return (Pnx1 - Pnx0) / n;
}

/**
 * @brief Computes the derivative of the Legendre polynomial of degree n at a given point.
 *
 * This function calculates the derivative of the Legendre polynomial of degree n at a given point
 * using the recurrence relation:
 *
 * P'_n(x) = n * (x * P_n(x) - P_{n-1}(x)) / (x^2 - 1)
 *
 * @tparam T The data type of the input point and the output derivative value.
 * @param n The degree of the Legendre polynomial.
 * @param point The point at which the derivative of the Legendre polynomial is to be evaluated.
 * @return The derivative of the Legendre polynomial of degree n at the given point.
 */
template <typename T> static T getLegendrePolynomialDerivative(const size_t n, const T point) {
    const T D1 = point * getLegendrePolynomial(n, point);
    const T D0 = getLegendrePolynomial(n - 1, point);
    return (n * (D1 - D0)) / (std::pow(point, 2.0f) - 1.0f);
}

/**
 * @brief Calculates the Gauss-Legendre weight for a given node and polynomial degree.
 *
 * This function computes the weight associated with a Gauss-Legendre quadrature node for a given
 * Legendre polynomial degree. The weight is used in numerical integration to approximate the
 * definite integral of a function.
 *
 * @tparam T The data type of the node and weight, typically float or double.
 * @param n The degree of the Legendre polynomial.
 * @param node The Gauss-Legendre quadrature node for which the weight is to be calculated.
 * @return The Gauss-Legendre weight for the given node and polynomial degree.
 *
 * @note The function assumes that the input node is a valid Gauss-Legendre quadrature node for the
 * given polynomial degree. It does not perform any checks for the validity of the input node.
 *
 * @see getLegendrePolynomialDerivative() for the calculation of the Legendre polynomial derivative.
 */
template <typename T> static inline T getGaussLegendreWeight(const size_t n, const T node) {
    const T derivative = getLegendrePolynomialDerivative(n, node);
    const T weightDenominator = (1.0f - std::pow(node, 2.0f)) * std::pow(derivative, 2.0f);
    const T weight = 2.0f / weightDenominator;
    return weight;
}

/**
 * @brief Fills the provided weights vector with the Gauss-Legendre weights corresponding to the given nodes.
 *
 * This function calculates the Gauss-Legendre weights for the given nodes using the getGaussLegendreWeight function
 * and stores the results in the provided weights vector. The weights vector is resized to match the size of the nodes
 * vector.
 *
 * @tparam T The data type of the nodes and weights (e.g., double, float).
 * @param[out] weights A std::vector<T> to store the calculated Gauss-Legendre weights.
 * @param[in] nodes A const std::vector<T> containing the Gauss-Legendre nodes for which the weights are calculated.
 * @param[in] n The size_t representing the number of nodes and weights.
 */
template <typename T>
static void fillGaussLegendreWeights(std::vector<T> &weights, const std::vector<T> &nodes, const size_t n) {
    std::transform(nodes.begin(), nodes.end(), weights.begin(),
                   [n](const T &node) { return getGaussLegendreWeight(n, node); });
}

/**
 * @brief Computes the Gauss-Legendre node for a given index and polynomial degree.
 *
 * The roots of the Legendre polynomial of degree n are symmetric around the origin and are located in the interval
 * [-1, 1]. They are also close to the zeros of the Chebyshev polynomials of the first kind, which are given by:
 *
 *  x_i = cos((2i - 1) * π / (2n))
 *
 * However, the roots of the Legendre polynomial are slightly shifted towards the center compared to the zeros of the
 * Chebyshev polynomial. To account for this shift, we can use the following formula as the initial guess for the
 * Newton-Raphson method:
 *
 *  x_i = cos(π * (i - 0.25) / (n + 0.5))
 *
 * This function will fill with Chebyshev polynomial guesses and iterate towards finding the roots using the
 * Newton-Raphson method.
 *
 * @tparam T The numeric type of the Gauss-Legendre node.
 * @param index The index of the Gauss-Legendre node.
 * @param n The degree of the Legendre polynomial.
 * @return The Gauss-Legendre node for the given index and polynomial degree.
 */
template <typename T> static T getGaussLegendreNode(const size_t index, const size_t n) {
    const T x_guess = std::cos(M_PI * (static_cast<T>(index) - 0.25f) / (static_cast<T>(n) + 0.5f));
    const std::function<T(T)> f = [n](T x) { return getLegendrePolynomial(n, x); };
    std::function<T(T)> df = [n](T x) { return getLegendrePolynomialDerivative(n, x); };
    return RootFinder::usingNewtonRaphson(f, df, x_guess);
}

/**
 * @brief Fills a vector with the Gauss-Legendre nodes for a given number of nodes.
 *
 * This function calculates the Gauss-Legendre nodes for a given number of nodes and stores them in a vector.
 * The Gauss-Legendre nodes are the roots of the Legendre polynomials and are used in numerical integration.
 *
 * @tparam T The data type of the nodes (e.g., double, float).
 * @param[out] nodes A vector to store the calculated Gauss-Legendre nodes.
 * @param[in] n The number of Gauss-Legendre nodes to calculate.
 *
 * @note The function clears and resizes the input vector before filling it with the calculated nodes.
 * @note The function uses the getGaussLegendreNode() function to calculate each individual node.
 */
template <typename T> static void fillGaussLegendreNodes(std::vector<T> &nodes, const size_t n) {
    nodes.clear();
    nodes.resize(n);
    for (size_t i = 1; i <= n; i++) {
        nodes[i - 1] = getGaussLegendreNode<T>(i, n);
    }
}

/**
 * @brief Computes the integral of a user-defined function using Gauss-Legendre quadrature.
 *
 * This function calculates the integral of a user-defined function over the interval [a, b] using
 * Gauss-Legendre quadrature with n quadrature points. The quadrature points and weights are
 * calculated using fillGaussLegendreNodes() and fillGaussLegendreWeights() functions.
 *
 * @param[out] outputs A reference to a GaussLegendreOutputs structure that will store the computed
 *                     integral, quadrature points, and weights.
 * @param[in]  inputs  A constant reference to a NewtonCotesInputs structure containing the input
 *                     parameters for the quadrature, such as the number of quadrature points (n),
 *                     the interval [a, b], and a flag to flip the integral sign.
 *
 * @return The computed integral value as a long double.
 *
 * @note The user-defined function to be integrated should be defined as user_defined_fx().
 * @note The Stopwatch<Nanoseconds> timer is used to measure the time taken for the computation.
 */
static long double fillUsingGaussianQuadratures(GaussLegendreOutputs &outputs, const NewtonCotesInputs &inputs) {

    const auto n = inputs.n;
    const auto a = inputs.a;
    const auto b = inputs.b;

    long double totalTime = 0.0f;
    for (size_t runs = 0; runs < 1000; runs++) {

        Stopwatch<Nanoseconds> timer;
        timer.restart();

        auto nodes = std::vector<long double>(n);
        fillGaussLegendreNodes(nodes, n);

        auto weights = std::vector<long double>(n);
        fillGaussLegendreWeights(weights, nodes, n);

        long double integral = 0.0f;
        for (size_t i = 0; i < n; i++) {
            const long double x_i = 0.5f * ((b - a) * nodes[i] + a + b);
            integral += weights[i] * user_defined_fx(x_i);
        }

        integral *= 0.5f * (b - a);
        timer.click();

        outputs.integral = integral;
        outputs.weights = weights;
        outputs.quadrature_points = nodes;

        totalTime += static_cast<long double>(timer.duration().count());
    }

    std::cout << "Computing 1000 integrals using Gauss-Legendre Quadrature for n=(" << n << "), degree-" << (n - 1)
              << " took " << (totalTime) << " ns, avg: " << (totalTime) / 1000.f << std::endl;

    if (inputs.flip_integral) {
        outputs.integral = -outputs.integral;
    }

    return outputs.integral;
}
