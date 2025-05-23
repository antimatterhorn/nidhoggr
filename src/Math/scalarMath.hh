#pragma once

#include <vector>
#include <cmath>

std::vector<double> logspace(double logStart, double logEnd, size_t num) {
    std::vector<double> result(num);
    double delta = (logEnd - logStart) / (num - 1);
    for (size_t i = 0; i < num; ++i) {
        result[i] = logStart + i * delta;
    }
    return result;
}