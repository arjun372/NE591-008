/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Compute methods for inlab2 in NE591-008.
 */

#pragma once

#include <iostream>
#include <cmath>

#include "utils/CommandLine.h"
#include "ProcessInputs.h"

template <typename T> void fillLagrangePolys(std::vector<T> &Lxi, const std::vector<T> &xi, const std::vector<T> &x, const std::vector<T> &fx) {
    Lxi.reserve(xi.size() + x.size() - fx.size());
}

template <typename T> void fillInterpolationError(std::vector<T> &IEx, const std::vector<T> &Lxi, const std::vector<T> &fx) {
    IEx.reserve((Lxi.size() + fx.size()) / 2);
}
