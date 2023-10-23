/**
* @file DiffusionConstants.h
* @brief This file contains the Constants struct which is used to store and compute various constants related to the
* diffusion process.
* @author Arjun Earthperson
* @date 10/13/2023
* @details The Constants struct is a template structure that can be used with any data type. It computes and stores
* various constants related to the diffusion process such as diffusion coefficient, cross section, and others.
*/

#ifndef NE591_008_DIFFUSIONCONSTANTS_H
#define NE591_008_DIFFUSIONCONSTANTS_H

#include <cstddef>
#include <ostream>
#include "DiffusionParams.h"

/**
 * @namespace MyPhysics::Diffusion
 * @brief This namespace contains all the classes and functions related to the diffusion process.
 */
namespace MyPhysics::Diffusion {

/**
 * @struct Constants
 * @brief This struct is used to compute and store various constants related to the diffusion process.
 * @tparam T The data type of the constants.
 */
template <typename T>
struct Constants {
    /**
     * @brief The number of rows in the diffusion grid.
     */
    size_t m;
    /**
     * @brief The number of columns in the diffusion grid.
     */
    size_t n;
    /**
     * @brief The total size of the diffusion grid (m*n).
     */
    size_t size;
    /**
     * @brief The diffusion coefficient.
     */
    T D;
    /**
     * @brief The diffusion coefficient divided by the square of the grid spacing.
     */
    T D_over_delta_squared;
    /**
     * @brief The negative of the diffusion coefficient divided by the square of the grid spacing.
     */
    T minus_D_over_delta_squared;
    /**
     * @brief The diffusion coefficient divided by the square of the absorption coefficient.
     */
    T D_over_gamma_squared;
    /**
     * @brief The negative of the diffusion coefficient divided by the square of the absorption coefficient.
     */
    T minus_D_over_gamma_squared;
    /**
     * @brief The macroscopic removal cross section.
     */
    T cross_section;
    /**
     * @brief The diagonal element of the diffusion matrix.
     */
    T diagonal;

    /**
     * @brief Overloaded stream insertion operator to print the Constants object.
     * @param os The output stream.
     * @param constants The Constants object to be printed.
     * @return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Constants &constants) {
        os << "m: " << constants.m << " n: " << constants.n << " D: " << constants.D
           << " D_over_delta_squared: " << constants.D_over_delta_squared
           << " minus_D_over_delta_squared: " << constants.minus_D_over_delta_squared
           << " D_over_gamma_squared: " << constants.D_over_gamma_squared
           << " minus_D_over_gamma_squared: " << constants.minus_D_over_gamma_squared
           << " cross_section: " << constants.cross_section << " diagonal: " << constants.diagonal;
        return os;
    }

    /**
     * @brief Computes the constants based on the provided diffusion parameters.
     * @param params The diffusion parameters.
     * @return The computed Constants object.
     */
    static Constants compute(const Params<T>& params) {
        Constants constants;
        constants.m = params.getM();
        constants.n = params.getN();
        constants.size = constants.m * constants.n;
        constants.D = params.getDiffusionCoefficient();
        constants.D_over_delta_squared = constants.D / std::pow(params.getDelta(), static_cast<T>(2));
        constants.minus_D_over_delta_squared = -constants.D_over_delta_squared;
        constants.D_over_gamma_squared = constants.D / std::pow(params.getGamma(), static_cast<T>(2));
        constants.minus_D_over_gamma_squared = -constants.D_over_gamma_squared;
        constants.cross_section = params.getMacroscopicRemovalCrossSection();
        constants.diagonal = static_cast<T>(2) * (constants.D_over_delta_squared + constants.D_over_gamma_squared) + constants.cross_section;
        return constants;
    }
};

}

#endif // NE591_008_DIFFUSIONCONSTANTS_H
