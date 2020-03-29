//
// Created by myralllka on 3/25/20.
//
#include "../includes/linear_program.h"
#include "../includes/linear_extractor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <boost/locale.hpp>

//namespace locale=boost::locale;

void count_words(std::string &input_filename, std::string &output_filename_a, std::string &output_filename_n) {
    // read entire binary archive into the buffer
    std::ifstream raw_file(input_filename, std::ios::binary);
    std::vector<std::string> data;
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    data = extract_to_memory(buffer);
    for (auto &element:data) {
        element = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(element)));
        std::cout << element << std::endl;
    }
    // ##########################################################
    // IN PROCESS (DIFFERENT TESTING)
    // ##########################################################
    // ##########################################################
    //check all existing lbm`s
//    boost::locale::localization_backend_manager lbm
//            = boost::locale::localization_backend_manager::global();
//    auto s = lbm.get_all_backends();
//    for_each(s.begin(), s.end(),
//             [](std::string& x){ std::cout << x << std::endl; });
    // ##########################################################
}
