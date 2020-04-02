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

//namespace locale=boost::locale;

void count_words(const std::string &input_filename, const std::string &output_filename_a,
                 const std::string &output_filename_n) {
    // read entire binary archive into the buffer
    std::ifstream raw_file(input_filename, std::ios::binary);
    std::vector<std::string> data;
    std::string word;
    std::map<std::string, int> map_of_words;
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    data = extract_to_memory(buffer);
    std::cout << "files num: " << data.size() << std::endl;
    for (auto &element : data) {
        element = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(element)));
        element.erase(std::remove_if(element.begin(), element.end(),
                                     [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), element.end());
        for (auto &chr:element) {
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
    for (auto &pair:map_of_words) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

//    for(auto elem : )
//    {
//        std::cout << elem.first << " " << elem.second.first << " " << elem.second.second << "\n";
//    }
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
