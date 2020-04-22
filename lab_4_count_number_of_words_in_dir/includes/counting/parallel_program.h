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

void parallel_count(t_queue<file_packet> *loader_queue, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t number_of_threads);


template<class S, class T>
void read_files_thread(const S &file_list, T *data_struct) {
    for (const auto &file_name : file_list) {
        read_input_file_gen(file_name, data_struct);
    }
    data_struct->push_back(file_packet{});
}

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
