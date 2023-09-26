/**
 * @file CheckBounds.h
 * @brief This file contains the declaration of functions for checking the validity of input parameters.
 * @date 08/30/2023
 * @author Arjun Earthperson
 */

#ifndef NE591_008_CHECKBOUNDS_H
#define NE591_008_CHECKBOUNDS_H

#include <iostream>
#include <boost/program_options.hpp>

#include "CommandLine.h"

/**
 * @brief Checks if the vector stored in the boost::program_options::variables_map under the given key is unfilled.
 *
 * This function attempts to retrieve a vector of type T from the variables_map using the provided key.
 * It then checks if the size of the retrieved vector is less than the expected size, indicating that the vector is
 * unfilled. If any exception occurs during this process (e.g., the key does not exist in the map, or the value
 * associated with the key is not a vector of type T), the function catches the exception and returns true, indicating
 * that the vector is considered as unfilled.
 *
 * @tparam T The type of the elements in the vector.
 * @param values The boost::program_options::variables_map from which to retrieve the vector.
 * @param key The key associated with the vector in the variables_map.
 * @param expectedSize The expected size of the vector.
 * @return true if the vector is unfilled (i.e., its size is less than the expected size or an exception occurred),
 * false otherwise.
 */
template <typename T>
static bool isUnfilledVector(boost::program_options::variables_map &values, const std::string &key, const size_t expectedSize) {
    try {
        return values[key].as<std::vector<T>>().size() < expectedSize;
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

/**
 * @brief Converts all characters in a given string to lowercase.
 *
 * @param mixedCaseStr A reference to the input string with mixed case characters.
 */
static void toLowerCase(std::string &mixedCaseStr) {
    for (char& c : mixedCaseStr) {
        c = std::tolower(c);
    }
}

/**
 * @brief Removes all spaces from a given string.
 *
 * @param spaceyStr A reference to the input string containing spaces.
 */
static void stripSpaces(std::string &spaceyStr) {
    spaceyStr.erase(std::remove_if(spaceyStr.begin(), spaceyStr.end(), ::isspace), spaceyStr.end());
}

/**
 * @brief Converts a given string to a boolean value based on its content.
 *
 * The function accepts "yes", "y", "no", and "n" as valid inputs (case-insensitive).
 * If the input is not one of these values, an exception is thrown.
 *
 * @param input A reference to the input string.
 * @return true If the input string is "yes" or "y".
 * @return false If the input string is "no" or "n".
 * @throws std::exception If the input string is not one of the valid values.
 */
static bool asYesOrNo(std::string &input) {
    toLowerCase(input);
    stripSpaces(input);
    if(input == "y" || input == "yes") {
        return true;
    }

    if(input == "n" || input == "no") {
        return false;
    }

    std::cerr << "Unknown value, please specify one of: [yes,y,no,n]\n";
    throw std::exception();
}

#endif //NE591_008_CHECKBOUNDS_H
