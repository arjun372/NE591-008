/*
 * Arjun Earthperson
 * Variable bounds checking helper functions for lab projects in NE591-008.
*/

#ifndef NE591_008_BOUNDSCHECK_H
#define NE591_008_BOUNDSCHECK_H

#include <iostream>
#include <iomanip>
#include <limits>

#include <boost/program_options.hpp>

const auto default_precision {std::cout.precision()};
constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};

template<class T>
void replace(std::map<std::string, boost::program_options::variable_value>& vm, const std::string& opt, const T& val)
{
    vm[opt].value() = boost::any(val);
}

template <typename Generic> struct Bounds {
    Generic min;
    Generic max;
};

//template <typename Generic> bool failsBoundsCheck(Generic &value, Bounds<Generic> bounds) {
//
//}

#endif //NE591_008_BOUNDSCHECK_H
