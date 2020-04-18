//
// Created by myralllka on 3/25/20.
//
#include "../../includes/speed_tester.h"
#include "../../includes/files/file_interface.h"
#include "../../includes/counting/word_count.h"
#include <iostream>
#include <map>
#include <string>
#include <algorithm>

void linear_count(const std::vector<std::string> &file_names, const std::string &output_filename_a,
                  const std::string &output_filename_n) {
    std::map<std::string, size_t> map_of_words;
    std::string word;

    auto analyzing_time = get_current_time_fenced();
    std::vector<file_packet> file_buf{};
    for (const std::string &file_n : file_names) {
        read_input_file(file_n, &file_buf);
        for (auto &file : file_buf) {
            alternative_word_count(std::move(file.content), &map_of_words);
        }
        file_buf.clear();
    }
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
    print(map_of_words, output_filename_a, output_filename_n);
}
