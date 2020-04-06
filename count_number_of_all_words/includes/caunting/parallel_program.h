//
// Created by solia on 4/5/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H

#include <string>
#include <vector>
#include <map>
#include "../queues/tqueue.h"


void count_words(std::string &data, size_t start_position, size_t end_position,
                 t_queue<std::map<std::string, int>> &queue);

void parallel_count(std::string &input_filename, std::string &output_filename_a,
                    std::string &output_filename_n, uint8_t num_threads);

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
