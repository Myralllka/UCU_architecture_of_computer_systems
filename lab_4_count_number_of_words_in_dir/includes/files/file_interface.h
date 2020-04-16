//
// Created by fenix on 4/6/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H
#define COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H

#include <string>
#include <map>
#include <algorithm>
#include "../../includes/archivation/archive_t.h"

void print(const std::map<std::string, size_t> &map_of_words, const std::string &output_filename_a,
           const std::string &output_filename_n);

std::string read_binary_file(const std::string &filename);

template<class T>
void read_input_file(const std::string &input_filename, T *data_struct) {
    if (input_filename.substr(input_filename.find_last_of('.') + 1) == "txt") {
        data_struct->emplace_back(read_binary_file(input_filename));
    } else {
        archive_t::extract_to(input_filename, data_struct);
    }
}


#endif //COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H
