//
// Created by solia on 4/5/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H

#include <string>
#include <vector>
#include <map>
#include "tqueue.h"

void count_words(const std::string &data, const size_t start_position, const size_t end_position, t_queue<std::map<std::string, int>> &queue);
//void count_words(std::string &data, const int start_position, const int end_position, t_queue<std::map<std::string, int>> &queue);
void merge_maps_queue(t_queue<std::map<std::string, int>> &queue);

void parallel_count(const std::string &input_filename, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t num_threads);

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
