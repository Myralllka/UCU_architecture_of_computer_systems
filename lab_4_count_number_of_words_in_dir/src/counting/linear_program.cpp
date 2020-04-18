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
    std::chrono::duration<long, std::ratio<1, 1000000000>> load_time{};

    auto analyzing_time = get_current_time_fenced();
    std::vector<file_packet> file_buf{};
    for (const std::string &file_n : file_names) {
        //////////////// FILE LOAD and UNARCHIVE /////////////
        auto start_load = get_current_time_fenced();
        read_input_file_gen(file_n, &file_buf); // generic method to load all files
        load_time += get_current_time_fenced() - start_load;
        //////////////////////////////////////////////////////

        for (const auto &file : file_buf) {
            fast_count_words(file.content, &map_of_words);
        }
        file_buf.clear();
    }
#ifdef DEBUG_INFO
    std::cout << "Loading: " << to_s(load_time) << std::endl;
    std::cout << "Analyzing: " << to_s(get_current_time_fenced() - analyzing_time) << std::endl;
#else
    std::cout << "Loading: " << to_us(load_time) << std::endl;
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
#endif

    dump_map_to_files(map_of_words, output_filename_a, output_filename_n);
}
