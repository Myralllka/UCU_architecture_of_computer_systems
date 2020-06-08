//
// Created by myralllka on 6/1/20.
//

#ifndef LAB_2_PARALLEL_INTEGRATION_ARGUMENTS_H
#define LAB_2_PARALLEL_INTEGRATION_ARGUMENTS_H

#include <vector>
#include <cstddef>

struct point {
    double x;
    double y;
};

struct f_params {
    const size_t m;
    const std::vector<double> c;
    const std::vector<double> a1;
    const std::vector<double> a2;
};

struct integration_args {
    point start = point{0.0, 0.0};
    point end = point{0.0, 0.0};
    const f_params conf{};
    const unsigned int flow_n = 1;
    double dx = 0.0, dy = 0.0;
};

#endif //LAB_2_PARALLEL_INTEGRATION_ARGUMENTS_H
