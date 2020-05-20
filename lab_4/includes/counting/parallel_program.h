//
// Created by myralllka on 4/5/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H

#include <string>
#include <vector>
#include <map>
#include "../files/file_packet.h"
#include "../files/file_interface.h"
#include "../../includes/code_control.h"
#include "../../includes/queues/tqueue.h"

void counting(t_queue<file_packet> &file_q,
              t_queue<std::map<std::string, size_t>> &map_q);

void merge_maps(
        t_queue<std::map<std::string, size_t>> &queue, uint8_t num_of_threads);

template<class S, class T>
void read_files_thread(const S &file_list, T &data_struct) {
    size_t f_size = 0;
    for (const auto &file_name : file_list) {
        read_input_file_gen(file_name, data_struct);
    }
    data_struct.emplace_back(file_packet{});
}

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
