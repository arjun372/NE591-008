/**
* @file Stats.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief This file contains the declaration of statistical functions and structures for vectors and matrices.
*/

#ifndef NE591_008_STATS_H
#define NE591_008_STATS_H

#include <algorithm>
#include <cmath>
#include <numeric>
#include <ostream>
#include <vector>

#include "BLAS.h"
#include "Matrix.h"
#include "Vector.h"

/**
 * @namespace MyBLAS::Stats
 * @brief A namespace for statistical functions and structures.
 */
namespace MyBLAS::Stats {

/**
 * @struct Summary
 * @brief A structure to hold the summary statistics of a vector or matrix.
 * @tparam T The type of the elements in the vector or matrix.
 */
template <typename T>
struct Summary {
    T min = std::numeric_limits<T>::quiet_NaN(); ///< The minimum value.
    T max = std::numeric_limits<T>::quiet_NaN(); ///< The maximum value.
    T sum = std::numeric_limits<T>::quiet_NaN(); ///< The sum of all values.
    T mean = std::numeric_limits<T>::quiet_NaN(); ///< The mean value.
    T variance = std::numeric_limits<T>::quiet_NaN(); ///< The variance.
    T stddev = std::numeric_limits<T>::quiet_NaN(); ///< The standard deviation.
    T median = std::numeric_limits<T>::quiet_NaN(); ///< The median value.
    T p5th = std::numeric_limits<T>::quiet_NaN(); ///< The 5th percentile.
    T P95th = std::numeric_limits<T>::quiet_NaN(); ///< The 95th percentile.

    /**
     * @brief Overloaded stream insertion operator to print the summary statistics.
     * @param os The output stream.
     * @param summary The summary statistics to print.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Summary &summary) {
        auto precision = std::cout.precision() ;
        const auto width = static_cast<int>(2 + static_cast<std::streamsize>(10));
        os << std::setprecision(2) << std::setw(width) << std::setfill(' ') << std::scientific;
        os << ":::::: SUM: "<<summary.sum<<" :::::::: VARIANCE: "<<summary.variance<<" :::::::: MEDIAN: "<<summary.median<<" :::::\n";
        os << ":::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::\n";
        os << ":::::: {" << summary.min << ", " << summary.max << "} ";
        os << ": (" << summary.mean << " ± " << summary.stddev << ") : [";
        os << summary.p5th << ", "<< summary.P95th << "] :::::";
        os << std::setprecision(precision);
        return os;
    }
};

/**
 * @brief Function to find the minimum value in a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @return The minimum value.
 */
template <typename T>
T min(const Vector<T>& v) {
    auto data = v.getData();
    if (data.empty()) {
        return NAN;
    }
    return *std::min_element(data.begin(), data.end());
}

/**
 * @brief Function to find the maximum value in a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @return The maximum value.
 */
template <typename T>
T max(const Vector<T>& v) {
    auto data = v.getData();
    if (data.empty()) {
        return NAN;
    }
    return *std::max_element(data.begin(), data.end());
}

/**
 * @brief Function to find the absolute value of each element in a vector.
 * @tparam T The type of the elements in the vector.
 * @param a The vector.
 * @return A vector with the absolute value of each element.
 */
template <typename T> Vector<T> abs(const Vector<T> &a) {
    Vector<T> result(a.size(), 0);
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = fabsl(a[i]);
    }
    return result;
}

/**
 * @brief Function to find the absolute value of each element in a matrix.
 * @tparam T The type of the elements in the matrix.
 * @param a The matrix.
 * @return A matrix with the absolute value of each element.
 */
template <typename T> Matrix<T> abs(const Matrix<T> &a) {
    Matrix<T> result(a.getRows(), a.getCols(), 0);
    for (size_t i = 0; i < a.getRows(); ++i) {
        for (size_t j = 0; j < a.getCols(); ++j) {
            result[i][j] = fabsl(a[i][j]);
        }
    }
    return result;
}

/**
 * @brief Function to find the sum of all elements in a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @return The sum of all elements.
 */
template <typename T>
T sum(const MyBLAS::Vector<T> &v) {
    auto data = v.getData();
    return std::accumulate(data.begin(), data.end(), static_cast<T>(0));
}

/**
 * @brief Function to find the mean value of a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @return The mean value.
 */
template <typename T>
T mean(const MyBLAS::Vector<T> &v) {
    return MyBLAS::Stats::sum(v) / static_cast<T>(v.getData().size());
}

/**
 * @brief Function to find the variance of a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @return The variance.
 */
template <typename T>
T variance(const MyBLAS::Vector<T> &v) {
    T m = MyBLAS::Stats::mean(v);
    T sum = static_cast<T>(0);
    auto data = v.getData();
    std::for_each(data.begin(), data.end(), [&](long double x) { sum += (x - m) * (x - m); });
    return sum / data.size();
}

/**
 * @brief Function to find the standard deviation of a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @return The standard deviation.
 */
template <typename T>
T std(const MyBLAS::Vector<T> &v) {
    return std::sqrt(MyBLAS::Stats::variance(v));
}

/**
 * @brief Function to find the p-th percentile of a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @param p The percentile to find.
 * @return The p-th percentile.
 */
template <typename T>
T percentile(Vector<T> v, long double p) {
    auto data = v.getData();
    if (p < 0 || p > 100) {
        throw std::invalid_argument("Percentile must be between 0 and 100");
    }
    long double rank = (p / static_cast<long double>(100)) * (data.size() - static_cast<long double>(1));
    auto index = static_cast<size_t>(rank);
    long double fractional = rank - index;
    if (data.size() == 1) {
        return data[0];
    }
    std::nth_element(data.begin(), data.begin() + index, data.end());
    T val = data[index];
    if (fractional == 0 || index + 1 == data.size()) {
        return val;
    }
    std::nth_element(data.begin(), data.begin() + index + 1, data.end());
    return val + fractional * (data[index + 1] - val);
}

/**
 * @brief Function to find the median of a vector.
 * @tparam T The type of the elements in the vector.
 * @param v The vector.
 * @return The median.
 */
template <typename T>
T median(Vector<T> v) {
    return percentile(v, 50);
}

}
#endif // NE591_008_STATS_H
