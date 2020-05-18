//
// Created by solia on 4/5/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H

#include <string>
#include <vector>
#include <map>
#include "../queues/tqueue.h"
#include "../../includes/queues/tqueue_radio.h"
#include "../files/file_packet.h"
#include "../files/file_interface.h"

#include "../../includes/code_control.h"

#ifdef PROGRESS_BAR

#include "visualization.h"
#include <thread>
#include <iostream>
#include <atomic>

#endif

void parallel_count(t_queue<file_packet> *loader_queue, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t number_of_threads);


template<class S, class T>
void read_files_thread(const S &file_list, T *data_struct) {
#ifdef PROGRESS_BAR
    std::atomic_uint64_t count{0};
    volatile bool print_progress = true;
    std::thread visual_t{[](volatile bool *cond, std::atomic_uint64_t *num, size_t max_num) {
        while (*cond) {
            printProgress(static_cast<double>(num->load()) / max_num);
            sleep(1);
        }
    }, &print_progress, &count, file_list.size()};
#endif
    size_t f_size = 0;
    for (const auto &file_name : file_list) {
        read_input_file_gen(file_name, data_struct);
#ifdef PROGRESS_BAR
        count++;
#endif
    }
    data_struct->emplace_back(file_packet{});

#ifdef PROGRESS_BAR
    print_progress = false;
    visual_t.join();
#endif
}

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
