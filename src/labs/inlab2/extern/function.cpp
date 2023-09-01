
#include "function.h"


void fill_fx(const std::vector<long double> &x, std::vector<long double> &fx) {

    if (x.size() != fx.size()) {
        return;
    }

    // trivial
    for(size_t i = 0; i <= fx.size(); i++)
        fx[i] = 2.0f * x[i];
    }

}
