//
// Created by fenix on 2/25/20.
//

#ifndef LAB_2_PARALLEL_INTEGRATION_MANUAL_INTEGRATION_H
#define LAB_2_PARALLEL_INTEGRATION_MANUAL_INTEGRATION_H

#include <cstddef>
#include <cassert>
#include <thread>
#include <vector>
#include <immintrin.h>

#include "arguments.h"


// steps describe the number of steps for etch x and y separately
template<typename func_T>
void simple_integrate(func_T func, integration_args int_ars, double *res) {
    double l_res = 0.0; // local result
    // return zero area if invalid range of integration
    assert(int_ars.end.x > int_ars.start.x
           && int_ars.end.y > int_ars.start.y
           && "invalid integration boundaries in simple integrate");

    // point to calculate
    point tmp_p{int_ars.start.x, int_ars.start.y};

    while (tmp_p.y < int_ars.end.y) {
        while (tmp_p.x < int_ars.end.x) {
            l_res += func(tmp_p, int_ars.conf);
            tmp_p.x += int_ars.dx;
        }
        tmp_p.x = int_ars.start.x;
        tmp_p.y += int_ars.dy;
    }

    *res = l_res * int_ars.dx * int_ars.dy;
}

void simd_simple_integrate_langermann(const integration_args &int_ars, double *res) {
    double local_result = 0; // local result
    // return zero area if invalid range of integration
    assert(int_ars.end.x > int_ars.start.x
           && int_ars.end.y > int_ars.start.y
           && "invalid integration boundaries in simple integrate");

    // point to calculate
    point tmp_point{int_ars.start.x, int_ars.start.y};
    double tmp_local_result;
    const __m256d PI = {M_PI, M_PI, M_PI, M_PI};
    const __m256d int_ars_conf_c = {int_ars.conf.c[0], int_ars.conf.c[1], int_ars.conf.c[2], int_ars.conf.c[3]};

    while (tmp_point.y < int_ars.end.y) {
        while (tmp_point.x < int_ars.end.x) {
            __m256d COORS = {tmp_point.x, tmp_point.y, tmp_point.x, tmp_point.y};
            __m256d d12 = {int_ars.conf.a1[0], int_ars.conf.a2[0], int_ars.conf.a1[1], int_ars.conf.a2[1]};
            d12 = _mm256_sub_pd(COORS, d12);
            d12 = _mm256_mul_pd(d12, d12);
            __m256d d34 = {int_ars.conf.a1[2], int_ars.conf.a2[2], int_ars.conf.a1[3], int_ars.conf.a2[3]};
            d34 = _mm256_sub_pd(COORS, d34);
            d34 = _mm256_mul_pd(d34, d34);
            d12 = _mm256_hadd_pd(d12, d34);
            d34 = _mm256_div_pd(__m256d{-d12[0], -d12[2], -d12[1], -d12[3]}, PI);
            d12 = _mm256_mul_pd(d12, PI);
            __m256d COS = {cos(d12[0]), cos(d12[2]), cos(d12[1]), cos(d12[3])};
            d34 = _mm256_mul_pd(_mm256_mul_pd(COS, __m256d{exp(d34[0]), exp(d34[1]), exp(d34[2]), exp(d34[3])}), int_ars_conf_c);
            tmp_local_result = d34[0] + d34[1] + d34[2] + d34[3];

            
            auto diag = std::pow(tmp_point.x - int_ars.conf.a1[4], 2) + std::pow(tmp_point.y - int_ars.conf.a2[4], 2);

            tmp_local_result += int_ars.conf.c[4] * exp(-diag / M_PI) * cos(M_PI * diag);

            local_result -= tmp_local_result;
            tmp_point.x += int_ars.dx;
        }
        tmp_point.x = int_ars.start.x;
        tmp_point.y += int_ars.dy;
    }

    *res = local_result * int_ars.dx * int_ars.dy;
}
//                std::cout << r1[0] << " " << r1[1] << " " << r1[2] << " " << r1[3] << std::endl;
//                std::cout << r2[0] << " " << r2[1] << " " << r2[2] << " " << r2[3] << std::endl;


template<typename func_T>
double integrate(func_T func, size_t steps, const integration_args &int_ars) {
    assert(int_ars.flow_n != 0
           && int_ars.end.x > int_ars.start.x
           && int_ars.end.y > int_ars.start.y
           && "invalid integration boundaries");

    double res = 0.0;
    integration_args int_arg_template{int_ars};

    double dxy = sqrt((int_ars.end.x - int_ars.start.x) * (int_ars.end.y - int_ars.start.y) / steps);
    int_arg_template.dx = dxy;
    int_arg_template.dy = dxy;

    // one flow case
    if (int_ars.flow_n == 1) {
//        simple_integrate(func, int_arg_template, &res);
        simd_simple_integrate_langermann(int_arg_template, &res);
        return res;
    }

    std::vector<std::thread> v{};
    std::vector<double> v_res(int_ars.flow_n);
    for (auto &el : v_res) {
        el = 0.0;
    }

    steps /= int_ars.flow_n; // steps per thread per x or y

    for (uint16_t i = 0; i < int_ars.flow_n; ++i) {
        int_arg_template.start.x = int_ars.start.x + int_arg_template.dx * steps * i;
        int_arg_template.end.x = int_ars.start.x + int_arg_template.dx * steps * (i + 1);
//        v.emplace_back(simple_integrate<func_T>, func, int_arg_template, &(v_res[i]));
        v.emplace_back(simd_simple_integrate_langermann, int_arg_template, &(v_res[i]));
    }
    for (uint16_t i = 0; i < int_ars.flow_n; ++i) {
        v[i].join();
    }

    for (uint16_t i = 0; i < int_ars.flow_n; ++i) {
        res += v_res[i];
    }
    return res;
}

#endif //LAB_2_PARALLEL_INTEGRATION_MANUAL_INTEGRATION_H

#pragma clang diagnostic pop