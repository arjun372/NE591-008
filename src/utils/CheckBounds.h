/**
 * @file CheckBounds.h
 * @brief This file contains the declaration of functions for checking the validity of input parameters.
 * @date 08/30/2023
 * @author Arjun Earthperson
 */

#pragma once

#include <iostream>
#include <boost/program_options.hpp>

#include "CommandLine.h"


static bool isUnfilledDoubleLongVector(boost::program_options::variables_map &values, const std::string &key, const size_t expectedSize) {
    try {
        return values[key].as<std::vector<long double>>().size() < expectedSize;
    } catch (...) {
        return true;
    }
}

/**
 * @brief Checks if the absolute value of the input is greater than or equal to 1.
 *
 * @param value The input value to be checked.
 * @return true If the absolute value of the input is greater than or equal to 1.
 * @return false If the absolute value of the input is less than 1.
 */
static bool failsCheck1(long double value) {
    const long double max = 1.0f;
    if (abs(value) >= max) {
        std::cerr <<"Error: " << "abs(x) is greater than " << std::setprecision (19) << max << "\n";
        return true;
    }
    return false;
}

/**
 * @brief Checks if the input value is less than or equal to 0.
 *
 * @param value The input value to be checked.
 * @return true If the input value is less than or equal to 0.
 * @return false If the input value is greater than 0.
 */
static bool failsCheck2(long double value) {
    const long double min = 0.0f;
    if (value <= min) {
        std::cerr <<"Error: " << "error threshold is not greater than " << std::setprecision (19) << min << "\n";
        return true;
    }
    return false;
}

/**
 * @brief Checks if the input value is a positive natural number.
 *
 * @param value The input value to be checked.
 * @return true If the input value is not a positive natural number.
 * @return false If the input value is a positive natural number.
 */
static bool failsNaturalNumberCheck(long double value) {
    const long double min = 0;
    auto error = false;
    if (value <= min) {
        std::cerr <<"Error: " << "Input should be a positive number\n";
        error = true;
    }
    if (ceil(value) != floor(value)) {
        std::cerr <<"Error: " << "Input should be a natural number\n";
        error = true;
    }

    if (value == 0) {
        std::cerr <<"Error: " << "Input cannot be zero\n";
        error = true;
    }

    return error;
}

/**
 * @brief Converts a string to a long double number.
 *
 * This function attempts to convert a given string to a long double number.
 * If the string cannot be converted to a number (invalid argument), or if the
 * number is out of range for a double, an exception is thrown and an error message
 * is printed to the standard error stream.
 *
 * @param input The string to be converted to a long double number.
 * @return The converted long double number.
 * @throws std::invalid_argument If the string cannot be converted to a number.
 * @throws std::out_of_range If the number is out of range for a double.
 */
static long double asNumber(const std::string &input) {
    try {
        return std::stod(input);
    } catch (const std::invalid_argument &) {
        std::cerr << "Argument is invalid\n";
        throw;
    } catch (const std::out_of_range &) {
        std::cerr << "Argument is out of range for a double\n";
        throw;
    } catch (...) {
        throw;
    }
}
