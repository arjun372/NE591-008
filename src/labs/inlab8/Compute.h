/**
* @file Compute.h
* @author Arjun Earthperson
* @date 10/13/2023
* @brief This file contains the declaration and implementation of the Compute namespace, which provides a function to
 * compute the sum of a series.
*/

#ifndef NE591_008_INLAB8_COMPUTE_H
#define NE591_008_INLAB8_COMPUTE_H

/**
* @namespace Compute
* @brief This namespace provides a function to compute the sum of a series.
*/
namespace Compute {

/**
    * @brief Computes the sum of a series from a start to a stop index.
    *
    * This function computes the sum of a series from a start to a stop index. The series is defined as the square of
    * the ratio of (1.0f + log10(i-1)) to ((i-1) + log10(i)) for each i from start to stop. If start is greater than
    * stop, the function swaps the two values.
    *
    * @param start The start index of the series.
    * @param stop The stop index of the series.
    * @return The sum of the series from start to stop.
*/
MyBLAS::NumericType seriesSum(size_t start, size_t stop) {
   if (start > stop) {
       std::swap(start, stop);
   }
   MyBLAS::NumericType partialSum = 0;
   MyBLAS::NumericType previousLog = std::log10(start);
   for (size_t i = start + 1; i < stop; i++) {
       const MyBLAS::NumericType currentLog = std::log10(i);
       const MyBLAS::NumericType denominator = (i - 1) + currentLog;
       const MyBLAS::NumericType numerator = 1.0f + previousLog;
       previousLog = currentLog;
       const MyBLAS::NumericType term = std::pow(numerator / denominator, 2);
       partialSum += term;
   }
   return partialSum;
}
}
#endif // NE591_008_INLAB8_COMPUTE_H
