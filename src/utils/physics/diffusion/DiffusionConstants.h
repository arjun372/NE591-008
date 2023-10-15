
#ifndef NE591_008_DIFFUSIONCONSTANTS_H
#define NE591_008_DIFFUSIONCONSTANTS_H

#include <cstddef>
#include <ostream>
#include "DiffusionParams.h"

namespace MyPhysics::Diffusion {

template <typename T>
struct Constants {
    size_t m;
    size_t n;
    size_t size;
    T D;
    T D_over_delta_squared;
    T minus_D_over_delta_squared;
    T D_over_gamma_squared;
    T minus_D_over_gamma_squared;
    T cross_section;
    T diagonal;

    friend std::ostream &operator<<(std::ostream &os, const Constants &constants) {
        os << "m: " << constants.m << " n: " << constants.n << " D: " << constants.D
           << " D_over_delta_squared: " << constants.D_over_delta_squared
           << " minus_D_over_delta_squared: " << constants.minus_D_over_delta_squared
           << " D_over_gamma_squared: " << constants.D_over_gamma_squared
           << " minus_D_over_gamma_squared: " << constants.minus_D_over_gamma_squared
           << " cross_section: " << constants.cross_section << " diagonal: " << constants.diagonal;
        return os;
    }

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
