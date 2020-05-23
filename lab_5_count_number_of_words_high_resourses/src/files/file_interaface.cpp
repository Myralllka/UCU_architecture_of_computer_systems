//
// Created by fenix on 4/6/20.
//
#include "../../includes/files/file_interface.h"
#include <boost/filesystem.hpp>
#include <filesystem>
#include <iostream>
#include <sstream>
#include "../../includes/counting/map_helpers.h"


void dump_map_to_files(const std::map<std::string, size_t> &map_of_words, const std::string &output_filename_a,
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


std::string read_binary_file(const std::string &filename) {
    if (!boost::filesystem::exists(filename)) {
        std::cerr << "Error: File '" << filename << "' do not exist!" << std::endl;
        return std::string{};
    }
    std::ifstream raw_file(filename, std::ios::binary);
    std::ostringstream ss{};
    ss << raw_file.rdbuf();
    return ss.str();
}

// WARNING: do not list empty files!!!
void list_all_files_from(const std::string &dir_path, std::vector<std::string> &res) {
    for (const auto &file : std::filesystem::recursive_directory_iterator(dir_path)) {
        if (std::filesystem::is_regular_file(file) && !std::filesystem::is_empty(file)) {
            res.emplace_back(file.path().string());
        } else if (!std::filesystem::is_directory(file)) {
            std::cerr << "Warning directory contain not valid or empty files! '" << file.path().string()
                      << "' Is not counted!" << std::endl;
        }
    }
}

file_packet read_input_file_gen(const std::string &input_filename) {
    if (is_text_file(input_filename)) {
        if (is_of_allowed_size(input_filename)) {
            return file_packet{read_binary_file(input_filename)};
        } else {
            std::cerr << "Warning: text file '" << input_filename << "' is missed as it is empty or lager than "
                      << MAX_TEXT_FILE_SIZE << "!" << std::endl;
        }
    } else if (is_archive_file(input_filename)) {
        return file_packet{read_binary_file(input_filename), true};
    } else {
        std::cerr << "Warning: File '" << input_filename << "' passed as it has a not supported format!" << std::endl;
    }
    return file_packet{};
}
