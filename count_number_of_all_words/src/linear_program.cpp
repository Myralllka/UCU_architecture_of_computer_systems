//
// Created by myralllka on 3/25/20.
//
#include "../includes/linear_program.h"
#include "../includes/extractor.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include <map>
//#include <boost/locale.hpp>

//namespace locale=boost::locale;

void count_words(std::string &input_filename, std::string &output_filename_a, std::string &output_filename_n) {
    std::ifstream raw_file(input_filename, std::ios::binary);
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    std::cout << output_filename_a << output_filename_n << std::endl;
    std::string output;
    extract_to_memory(buffer, output);
    std::cout << output << std::endl;
}
