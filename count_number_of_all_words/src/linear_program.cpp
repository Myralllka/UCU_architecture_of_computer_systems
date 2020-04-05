//
// Created by myralllka on 3/25/20.
//
#include "../includes/linear_program.h"
#include "../includes/linear_extractor.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <boost/locale.hpp>
#include <algorithm>

void count_words(const std::string &input_filename, const std::string &output_filename_a,
                 const std::string &output_filename_n) {
    std::map<std::string, int> map_of_words;
    std::vector<std::string> data;
    std::string word;
    // read entire binary archive into the buffer

    extract_to_memory(read_binary_file_into_buffer(input_filename), &data);

    ////////////////debug//////////////////////
//    std::cout << "files num: " << data.size() << std::endl;
//    for (const auto &el : data) {
//        std::cout << el.size() << "###" << el << std::endl;
//    }
    ////////////////////////////////////////////
    for (auto &element : data) {
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
//        std::cout << element << std::endl;
    }
    std::ofstream outfile_alpha;
    std::ofstream outfile_number;
    outfile_alpha.open(output_filename_a);
    outfile_number.open(output_filename_n);

    for (auto &pair:map_of_words) {
        outfile_alpha << pair.first << ": " << pair.second << std::endl;
    }

    auto multimap_of_words = flip_map(map_of_words);

    for (auto &pair:multimap_of_words) {
        outfile_number << pair.first << ": " << pair.second << std::endl;
    }
    outfile_alpha.close();
    outfile_number.close();

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
    std::ifstream raw_file(filename, std::ios::binary);
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    return buffer;
}
