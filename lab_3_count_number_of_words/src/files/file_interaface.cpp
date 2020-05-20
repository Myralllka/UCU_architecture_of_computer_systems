//
// Created by fenix on 4/6/20.
//
#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>
#include "../../includes/files/file_interface.h"
#include "../../includes/merging/map_helpers.h"

void print(const std::map<std::string, size_t> &map_of_words, const std::string &output_filename_a,
           const std::string &output_filename_n) {
    std::ofstream outfile_alpha;
    std::ofstream outfile_number;
    outfile_alpha.open(output_filename_a);
    outfile_number.open(output_filename_n);

    for (auto &pair:map_of_words) {
        outfile_alpha << pair.first << "\t" << pair.second << std::endl;
    }

    auto multimap_of_words = flip_map(map_of_words);

    auto it = multimap_of_words.rbegin();
    while (it != multimap_of_words.rend()) {
        outfile_number << (*it).second << "\t" << (*it).first << std::endl;
        ++it;
    }

    outfile_alpha.close();
    outfile_number.close();
}


std::string read_binary_file_into_buffer(const std::string &filename) {
    if (!boost::filesystem::exists(filename)) {
        std::cerr << "Error: File '" << filename << "' do not exist!" << std::endl;
        return std::string{};
    }
    std::ifstream raw_file(filename, std::ios::binary);
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    return buffer;
}
