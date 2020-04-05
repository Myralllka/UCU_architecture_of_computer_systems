//
// Created by solia on 4/5/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H

#ifndef COUNT_NUMBER_OF_ALL_WORDS_LINEAR_PROGRAM_H
#define COUNT_NUMBER_OF_ALL_WORDS_LINEAR_PROGRAM_H

#include <string>

void count_words(const std::string &input_filename, const std::string &output_filename_a,
                 const std::string &output_filename_n, const int no_threads);

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
