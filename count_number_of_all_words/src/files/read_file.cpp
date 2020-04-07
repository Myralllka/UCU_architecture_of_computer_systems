//
// Created by myralllka on 4/7/20.
//

#include "../../includes/archivation/linear_extractor.h"
#include "../../includes/speed_tester.h"
#include "../../includes/files/file_interface.h"
#include "../../includes/files/read_file.h"
#include <iostream>
#include <string>
#include <boost/locale.hpp>
#include <fstream>

void read_input_file(const std::string &input_filename, std::vector<std::string> &data) {
    auto total_time = get_current_time_fenced();
    if (input_filename.substr(input_filename.find_last_of('.') + 1) == "txt") {
        std::ifstream f(input_filename);
        data.emplace_back(static_cast<std::ostringstream &>(std::stringstream{} << f.rdbuf()).str());
    } else {
        extract_to_memory(read_binary_file_into_buffer(input_filename), &data);
    }
    auto finish_time = get_current_time_fenced();
    std::cout << "Loading: " << to_us(finish_time - total_time) << std::endl;
}
