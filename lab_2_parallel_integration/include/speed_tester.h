//
// Created by fenix on 2/25/20.
//

#ifndef LAB_2_PARALLEL_INTEGRATION_SPEED_TESTER_H
#define LAB_2_PARALLEL_INTEGRATION_SPEED_TESTER_H

#include <chrono>

template<class D>
inline long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

std::chrono::steady_clock::time_point get_current_time_fenced();

#endif //LAB_2_PARALLEL_INTEGRATION_SPEED_TESTER_H
