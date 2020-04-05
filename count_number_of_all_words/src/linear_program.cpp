//
// Created by myralllka on 3/25/20.
//
#include "../includes/linear_program.h"
#include "../includes/linear_extractor.h"
#include "../includes/print_maps_to_files.h"
#include "../includes/speed_tester.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <boost/locale.hpp>
#include <algorithm>
#include <boost/filesystem.hpp>

void linear_count(const std::string &input_filename, const std::string &output_filename_a,
                  const std::string &output_filename_n) {
    std::map<std::string, int> map_of_words;
    std::vector<std::string> data;
    std::string word;
    // ##########################################################
    auto total_time = get_current_time_fenced();
    if (input_filename.substr(input_filename.find_last_of('.') + 1) == "txt") {
        std::ifstream f(input_filename);
        data.emplace_back(static_cast<std::ostringstream &>(std::ostringstream{} << f.rdbuf()).str());
    } else {
        extract_to_memory(read_binary_file_into_buffer(input_filename), &data);
    }
    auto finish_time = get_current_time_fenced();

    // ##########################################################
    auto analyzing_time = get_current_time_fenced();
//    for (auto &element : data) {
    auto element = data[0];
    element = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(element)));
    element.erase(std::remove_if(element.begin(), element.end(),
                                 [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), element.end());
    for (auto &chr : element) {
        if (isalpha(chr))
            word += tolower(chr);
        else if (isspace(chr)) {
            auto itr = map_of_words.find(word);
            if (itr != map_of_words.end()) {
                map_of_words[word] += 1;
            } else {
                map_of_words[word] = 1;
            }
            word.clear();
        }
    }
//    }
    std::cout << "Loading: " << to_us(finish_time - total_time) << std::endl;
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
    print(map_of_words, output_filename_a, output_filename_n);

    // ##########################################################
    // IN PROCESS (DIFFERENT TESTING)
    // ##########################################################
    //check all existing lbm`s
//    boost::locale::localization_backend_manager lbm
//            = boost::locale::localization_backend_manager::global();
//    auto s = lbm.get_all_backends();
//    for_each(s.begin(), s.end(),
//             [](std::string& x){ std::cout << x << std::endl; });
    // ##########################################################
}

std::string read_binary_file_into_buffer(const std::string &filename) {
//    if (!boost::filesystem::exists(filename)) {
//        boost::filesystem::path full_path(boost::filesystem::current_path());
//        std::cout << "Current path is : " << full_path << std::endl;
//        std::cout << "FIle name: " << filename << std::endl;
//        exit(111);
//    }
    std::ifstream raw_file(filename, std::ios::binary);
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    return buffer;
}
