
#include <iostream>
#include "function.h"


void fill_fx(const std::vector<long double> &x, std::vector<long double> &fx) {

    if (x.size() != fx.size()) {
        std::cerr<<"Warning: Vectors are of different lengths: size(x): "<<x.size()<<", size(f[x]): "<<fx.size()<<"\n";
    }

    // trivial
    for(size_t i = 0; i < x.size(); i++) {
        fx[i] = 2.0f * x[i];
    }

}
