/**
 * @file Ops.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the definition of the L2Norm function, which calculates the L2 norm (Euclidean distance) between two containers.
*/

#ifndef NE591_008_OPS_H
#define NE591_008_OPS_H

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <random>

namespace MyBLAS {

    /**
     * @brief Calculates the L2 norm (Euclidean distance) between two containers.
     *
     * For real numbers, the L2 norm is defined as sqrt(sum(v[i] * v[i])).
     * For complex numbers, the L2 norm is defined as sqrt(sum(v[i] * conj(v[i]))).
     *
     * @tparam ContainerType The type of the containers. This should be a container type that supports indexing with the [] operator.
     * @tparam DataType The type of the data contained in the containers. This should be a type that supports subtraction and multiplication.
     *
     * @param a The first container.
     * @param b The second container.
     * @param n The size of the containers. This function assumes that both containers have the same size.
     *
     * @return The L2 norm (Euclidean distance) between the two containers.
     */
    template <typename ContainerType, typename DataType>
    inline DataType L2Norm(const ContainerType &a, const ContainerType &b, const size_t n) {
        DataType norm = 0;
        for(size_t i = 0; i < n; i++) {
            const auto difference = a[i] - b[i];
            const auto squared = std::pow(difference, 2);
            norm += squared;
        }
        return norm;
    }

    // TODO:: DOCUMENT
    template <typename T>
    T L2Norm(const MyBLAS::Vector<T> &a, const MyBLAS::Vector<T> &b) {
        const size_t n = std::min(a.size(), b.size());
        return MyBLAS::L2Norm<MyBLAS::Vector<T>, T>(a, b, n);
    }
}
#endif //NE591_008_OPS_H
