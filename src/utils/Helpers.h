/**
 * @file Helpers.h
 * @author Arjun Earthperson
 * @date 08/30/2023
 * @brief This file contains helper functions and structures used throughout the program.
 */

#ifndef NE591_008_HELPERS_H
#define NE591_008_HELPERS_H

#include <iomanip>
#include <iostream>
#include <limits>

#include "json.hpp"
#include <boost/program_options.hpp>

/**
 * @brief This function replaces the value of a specified option in a map with a new value.
 *
 * @tparam T The type of the value to be replaced.
 * @param vm A map containing program options and their values.
 * @param opt The option whose value is to be replaced.
 * @param val The new value to replace the old value with.
 *
 * @note This function uses the boost library's any class to allow for type erasure,
 * meaning the function can accept any type for the new value.
 */
template <class T>
void replace(std::map<std::string, boost::program_options::variable_value> &vm, const std::string &opt, const T &val) {
    vm[opt].value() = boost::any(val);
}

/**
 * @brief Synchronizes the values of a given key between a JSON object and a Boost variables_map object.
 *
 * This function first checks if the key exists in the variables_map. If it does, the value is updated
 * in the JSON object. Then, if the key exists in the JSON object, the value is read from the JSON object
 * and updated in the variables_map.
 *
 * @tparam T The type of the value associated with the key.
 * @param key The key to synchronize between the JSON object and the variables_map.
 * @param inputMap The JSON object containing the key-value pairs.
 * @param map The Boost variables_map object containing the key-value pairs.
 */
template <typename T>
void syncMapKeys(std::string &key, nlohmann::json &inputMap, boost::program_options::variables_map &map) {
    // first, check if key is in the variables_map
    if (map.count(key)) {
        // if yes, update it in the JSON map.
        inputMap[key] = map[key].as<T>();
    }
    // then, if key is in the JSON map, read value from the JSON map into the variables_map.
    if (inputMap.contains(key)) {
        replace(map, key, static_cast<T>(inputMap[key]));
    }
}

/**
 * @brief This function fills a vector with evenly spaced values between a start and end value.
 *
 * @tparam T The type of the values to be generated.
 * @param result The vector to be filled with the generated values.
 * @param start The start value of the range.
 * @param end The end value of the range.
 * @param count The number of values to be generated.
 *
 * @note This function uses the std::generate function to generate the values.
 */
template <typename T> void fill_linspace(std::vector<T> &result, T start, T end, size_t count) {
    result.reserve(count);

    if (count <= 1) {
        result.clear();
        result[0] = start;
        return;
    }

    T step = (end - start) / static_cast<T>(count - 1);

    std::generate(result.begin(), result.end(), [&start, step]() mutable {
        T value = start;
        start += step;
        return value;
    });
}

/**
 * @brief Scales a value from one range to another.
 * @tparam T The data type of the value and range limits
 * @param value The value to be scaled
 * @param old_min The lower limit of the original range
 * @param old_max The upper limit of the original range
 * @param new_min The lower limit of the new range
 * @param new_max The upper limit of the new range
 * @return The scaled value in the new range
 */
template <typename T> static T scale(T value, T old_min, T old_max, T new_min, T new_max) {
    return (value - old_min) * (new_max - new_min) / (old_max - old_min) + new_min;
}

/**
 * @brief Calculates the number of iterations for a given point in the Julia set.
 * @tparam T The type of the complex numbers.
 * @param z0 The initial complex number.
 * @param c The constant complex number.
 * @param max_iterations The maximum number of iterations.
 * @return The number of iterations before the point escapes the Julia set.
 */
template <typename T>
size_t juliaSetIterations(const std::complex<T> &z0, const std::complex<T> &c, size_t max_iterations) {
    std::complex<T> z = z0;
    size_t iter = 0;
    for (; iter < max_iterations; iter++) {
        z = z * z + c;
        if (std::norm(z) > 4.0)
            break;
    }
    return iter;
}

/**
 * @brief A template structure for ToneMap.
 *
 * This structure is used to map tones using a scaling factor and a growth rate.
 * It is a template structure, so it can be used with any data type.
 *
 * @tparam T This is the type of the scaling factor and the growth rate. It can be any type that supports assignment and
 * arithmetic operations.
 */
template <typename T> struct ToneMap {
    /**
     * @brief The scaling factor for the tone map.
     *
     * This is the factor by which the input tone will be scaled.
     * It is initialized to a very small value (1e-7) to prevent large initial tones.
     */
    T scaling_factor = 1e-7;

    /**
     * @brief The growth rate for the tone map.
     *
     * This is the rate at which the tone map grows.
     * It is initialized to a moderate value (0.3) to allow for a reasonable growth rate.
     */
    T growth_rate = 0.3;
};

/**
 * @brief A struct representing the properties of a canvas for rendering ANSI characters
 * @tparam T The type of the numbers used for the canvas dimensions and properties.
 */
template <typename T> struct CanvasType {
    size_t width = 80;
    size_t height = 24;
    T x_start = -2;
    T y_start = -2;
    T x_stop = 2;
    T y_stop = 2;
    std::string character = "█";
    T contrast = 1.0;
    ToneMap<T> tone_map;
};
typedef CanvasType<__float128> Canvas;

/**
 * @brief Generates a vector of 256-bit ANSI gray shades.
 * @return A vector of ANSI escape codes representing gray shades.
 */
static std::vector<std::string> getGrays256bitANSI(double scaling_factor = 1e-7, double growth_rate = 0.3) {

    const long double x_min = 232;
    const long double x_max = 255;
    auto x = std::vector<long double>(26);
    fill_linspace(x, x_min, x_max, 26);

    auto y = std::vector<long double>(26);
    auto y_min = std::numeric_limits<long double>::max();
    auto y_max = std::numeric_limits<long double>::min();
    for (size_t i = 0; i < x.size(); i++) {
        y[i] = scaling_factor * std::exp(growth_rate * x[i]);
        if (y[i] > y_max) {
            y_max = y[i];
        }
        if (y[i] < y_min) {
            y_min = y[i];
        }
    }

    // 26 shades of gray
    std::vector<std::string> grayShades;
    grayShades.emplace_back("\033[38;5;016m"); // black
    for (size_t i = 0; i < 24; ++i) {
        int colorCode = 232 + i;
        grayShades.push_back("\033[38;5;" + std::to_string(colorCode) + "m");
    }
    grayShades.emplace_back("\033[38;5;231m"); // white

    std::vector<std::string> toneMap;
    for (size_t i = 0; i <= 25; ++i) {
        auto yi = 0 + (y[i] - y_min) * (25 - 0) / (y_max - y_min);
        auto idx = static_cast<int>(std::floor(yi));
        toneMap.push_back(grayShades[idx]);
    }
    return toneMap;
}

/**
 * @brief Generates a vector of 256-bit ANSI HSV colors.
 * @return A vector of ANSI escape codes representing HSV colors.
 */
static std::vector<std::string> getHSV256bitANSI() {
    std::vector<std::string> colors;

    // 1
    colors.emplace_back("\033[38;5;016m"); // black

    // 1 + 24
    for (int i = 232; i < 256; i++) {
        colors.push_back("\033[38;5;" + std::to_string(i) + "m");
    }

    // 1 + 24 + 1
    colors.emplace_back("\033[38;5;231m"); // white

    //
    std::vector<std::string> hues = {
        "225", "219", "218", "211", "212", "213", "207", "206", "200", "201", "165", "129", "093",
        "057", "021", "027", "033", "039", "045", "051", "087", "050", "049", "086", "085", "048",
        "047", "084", "083", "046", "082", "119", "155", "118", "154", "191", "190", "226", "227",
        "221", "220", "214", "208", "202", "160", "196", "197", "198", "197",
    };

    for (auto &hue : hues) {
        colors.push_back("\033[38;5;" + hue + "m");
    }

    return colors;
}

/**
 * @brief Prints the Julia set to the console using the specified canvas properties.
 * @tparam T The type of the numbers used for the complex numbers.
 * @param canvas The canvas properties for rendering the Julia set.
 * @param x0 The real part of the constant complex number.
 * @param y0 The imaginary part of the constant complex number.
 * @param max_iterations The maximum number of iterations (default is 120).
 */
template <typename T>
void printJuliaSet(const Canvas &canvas, const T x0, const T y0, const size_t max_iterations = 120) {

    const T x_range = fabsl(canvas.x_stop - canvas.x_start);
    const T y_range = fabsl(canvas.y_stop - canvas.y_start);
    T x_step = x_range / static_cast<T>(canvas.width);
    T y_step = y_range / static_cast<T>(canvas.height);

    const std::complex<T> c(x0, y0);

    T min = std::numeric_limits<T>::max();
    T max = std::numeric_limits<T>::min();
    T avg = 0.0;

    for (size_t y = 0; y < canvas.height; ++y) {
        for (size_t x = 0; x < canvas.width; ++x) {
            std::complex<T> z = std::complex<T>(canvas.x_start + x * x_step, canvas.y_start + y * y_step);
            size_t iter = juliaSetIterations(z, c, max_iterations);
            const T log_iter = std::log2(iter);
            if (log_iter == NAN || std::isinf(static_cast<long double>(log_iter))) {
                continue;
            }
            min = std::min(min, log_iter);
            max = std::max(max, log_iter);
            avg += log_iter;
        }
    }
    avg = avg / (canvas.width * canvas.height);

    std::vector<std::string> colors = getGrays256bitANSI(canvas.tone_map.scaling_factor, canvas.tone_map.growth_rate);
    for (size_t y = 0; y < canvas.height; ++y) {
        for (size_t x = 0; x < canvas.width; ++x) {
            std::complex<T> z = std::complex<T>(canvas.x_start + x * x_step, canvas.y_start + y * y_step);
            size_t iter = juliaSetIterations(z, c, max_iterations);
            T log_iter = std::log2(iter);
            T enhanced_value = (log_iter - avg) * canvas.contrast + avg;
            T color_index = scale<T>(enhanced_value, min, max, 0, colors.size());
            size_t clamped_index = std::clamp<size_t>(static_cast<size_t>(color_index), 0, colors.size() - 1);
            std::cout << colors[clamped_index] << canvas.character;
        }
        std::cout << '\n';
    }
    std::cout << "\033[0m";
}

#endif // NE591_008_HELPERS_H
