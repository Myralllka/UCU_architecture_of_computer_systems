//
// Created by fenix on 4/6/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H
#define COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H

#include <string>
#include <map>
#include <algorithm>


void print(const std::map<std::string, size_t> &map_of_words, const std::string &output_filename_a,
           const std::string &output_filename_n);

std::string read_binary_file_into_buffer(const std::string &filename);

#endif //COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H
