//
// Created by solia on 4/5/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H

#include <string>
#include <vector>
#include <map>
#include "../queues/tqueue.h"

void parallel_count(const std::string &input_filename, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t number_of_threads);

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
