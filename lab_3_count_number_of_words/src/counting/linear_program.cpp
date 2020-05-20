//
// Created by myralllka on 3/25/20.
//
#include "../../includes/archivation/linear_extractor.h"
#include "../../includes/speed_tester.h"
#include "../../includes/files/file_interface.h"
#include "../../includes/files/read_file.h"
#include <iostream>
#include <map>
#include <string>
#include <boost/locale.hpp>
#include <algorithm>

void linear_count(const std::vector<std::string> &data, const std::string &output_filename_a,
                  const std::string &output_filename_n) {
    std::map<std::string, size_t> map_of_words;
    std::string word;
//    for (auto &element : data) {
    auto element = data[0];
    element = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(element)));
    element.erase(std::remove_if(element.begin(), element.end(),[](const unsigned &c) { return !isspace(c) && !isalpha(c); }), element.end());
    auto analyzing_time = get_current_time_fenced();
    for (auto &chr : element) {
        if (isalpha(chr))
            word += tolower(chr);
        else if (isspace(chr) && !word.empty()) {
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
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
    print(map_of_words, output_filename_a, output_filename_n);
}
