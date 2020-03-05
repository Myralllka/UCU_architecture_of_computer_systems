//
// Created by fenix on 2/25/20.
//

#include "../include/speed_tester.h"
#include <atomic>
#include <cassert>


std::chrono::steady_clock::time_point get_current_time_fenced() {
    assert(std::chrono::steady_clock::is_steady &&
                   "Timer should be steady (monotonic).");
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::steady_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}
