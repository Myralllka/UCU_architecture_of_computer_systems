//
// Created by myralllka on 3/25/20.
//
#include "../../includes/files/file_interface.h"
#include "../../includes/counting/word_count.h"
#include <map>
#include <string>
#include <algorithm>
#include <boost/locale.hpp>

void linear_count(const std::vector<std::string> &file_names, const std::string &output_filename_a,
                  const std::string &output_filename_n) {
    std::map<std::string, size_t> map_of_words;
    std::vector<file_packet> file_buf{};

    for (const std::string &file_n : file_names) {
        read_input_file_gen(file_n, &file_buf); // generic method to load all files
        for (auto &file : file_buf) {
            std::string content{std::move(file.content)};
            /////////////////////// NORMALIZE CONTENT /////////////////////////
            content = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(content)));
            content.erase(std::remove_if(content.begin(), content.end(),
                                         [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), content.end());
            ///////////////////////////////////////////////////////////////////
            fast_count_words(content, &map_of_words);
        }
        file_buf.clear();
    }
    dump_map_to_files(map_of_words, output_filename_a, output_filename_n);
}
