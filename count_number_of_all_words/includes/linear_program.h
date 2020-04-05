//
// Created by myralllka on 3/25/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_LINEAR_PROGRAM_H
#define COUNT_NUMBER_OF_ALL_WORDS_LINEAR_PROGRAM_H

#include <string>

void linear_count(const std::string &input_filename, const std::string &output_filename_a,
                  const std::string &output_filename_n);

std::string read_binary_file_into_buffer(const std::string &filename);

#endif //COUNT_NUMBER_OF_ALL_WORDS_LINEAR_PROGRAM_H
