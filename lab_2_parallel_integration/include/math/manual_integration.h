//
// Created by fenix on 2/25/20.
//

#ifndef LAB_2_PARALLEL_INTEGRATION_MANUAL_INTEGRATION_H
#define LAB_2_PARALLEL_INTEGRATION_MANUAL_INTEGRATION_H

#include "integration_args.h"
#include <cstddef>
#include <cassert>
#include <thread>
#include <vector>


// steps describe the number of steps for etch x and y separately
template<typename func_T>
void simple_integrate(func_T func, size_t steps, integration_args int_ars, double *res) {
    double l_res = 0.0; // local result
    // return zero area if invalid range of integration
    assert(int_ars.end.x > int_ars.start.x
           && int_ars.end.y > int_ars.start.y
           && "invalid integration boundaries in simple integrate");

    double dx = (int_ars.end.x - int_ars.start.x) / sqrt(steps);
    double dy = (int_ars.end.y - int_ars.start.y) / sqrt(steps);
    // point to calculate
    point tmp_p{int_ars.start.x, int_ars.start.y};

    while (tmp_p.y < int_ars.end.y) {
        while (tmp_p.x < int_ars.end.x) {
            l_res += func(tmp_p, int_ars.conf);
            tmp_p.x += dx;
        }
        tmp_p.x = int_ars.start.x;
        tmp_p.y += dy;
    }

    *res = l_res * dx * dy;
}


template<typename func_T>
double integrate(func_T func, size_t steps, integration_args int_ars) {
    assert(int_ars.flow_n != 0
           && int_ars.end.x > int_ars.start.x
           && int_ars.end.y > int_ars.start.y
           && "invalid integration boundaries");
    double res = 0.0;
    // one flow case
    if (int_ars.flow_n == 1) {
        simple_integrate(func, steps, int_ars, &res);
        return res;
    }
    // integration block 'x' width
    double int_dx = (int_ars.end.x - int_ars.start.x) / int_ars.flow_n;

//    std::vector<std::thread> v{};
    std::vector<double> v_res(int_ars.flow_n);
    for (auto &el : v_res) {
        el = 0.0;
    }
    steps /= int_ars.flow_n; // steps per thread

    for (uint16_t i = 0; i < int_ars.flow_n; ++i) {
//        v.emplace_back(// create config structure for etch thread
//                simple_integrate<func_T>, func, steps,
//                integration_args{
//                        point{int_ars.start.x + int_dx * i, int_ars.start.y},
//                        point{int_ars.start.x + int_dx * (i + 1), int_ars.end.y},
//                        int_ars.conf,
//                        1
//                }, &(v_res[i]));
        simple_integrate(func, steps,
                         integration_args{
                                 point{int_ars.start.x + int_dx * i, int_ars.start.y},
                                 point{int_ars.start.x + int_dx * (i + 1), int_ars.end.y},
                                 int_ars.conf,
                                 1
                         }, &(v_res[i]));
    }

//    for (uint16_t i = 0; i < int_ars.flow_n; ++i) {
//        v[i].join();
//    }

    for (uint16_t i = 0; i < int_ars.flow_n; ++i) {
        res += v_res[i];
    }
    return res;
}

#endif //LAB_2_PARALLEL_INTEGRATION_MANUAL_INTEGRATION_H
