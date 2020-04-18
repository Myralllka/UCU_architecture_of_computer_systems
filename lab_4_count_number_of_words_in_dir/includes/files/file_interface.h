//
// Created by fenix on 4/6/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H
#define COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H

#include <string>
#include <map>
#include <algorithm>
#include <boost/filesystem.hpp>
#include "../../includes/archivation/archive_t.h"

#include "../../includes/debug_control.h"

void dump_map_to_files(const std::map<std::string, size_t> &map_of_words, const std::string &output_filename_a,
                       const std::string &output_filename_n);

std::string read_binary_file(const std::string &filename);

static inline std::string get_extension(const std::string &filename) {
    return filename.substr(filename.find_last_of('.') + 1);
}

static inline bool is_text_file(const std::string &filename) {
    return "txt" == get_extension(filename);
}

static inline bool is_archive_file(const std::string &filename) {
    static const std::vector<std::string> supported_formats = {"tar", "gz", "pax", "cpio", "zip", "mtree", "shar", "ar",
                                                               "raw", "xar", "lha", "lzh", "rar", "7z", "xz"};
    std::string extension = get_extension(filename);
    for (const auto &el : supported_formats)
        if (extension == el)
            return true;
    return false;
}

struct file_packet {
    bool archived;
    std::string content;

    file_packet(std::string content = "", bool compressed = false) : archived(compressed),
                                                                     content(std::move(content)) {}

    bool empty() const { return content.empty(); }
};

template<class T>
void read_input_file_gen(const std::string &input_filename, T *data_struct) {
#ifdef DEBUG_INFO
    std::cout << "load file: " << input_filename << std::endl;
#endif

    if (is_text_file(input_filename)) {
        data_struct->emplace_back(file_packet{read_binary_file(input_filename)});
    } else if (is_archive_file(input_filename)) {
        data_struct->emplace_back(file_packet{read_binary_file(input_filename), true});
    } else {
        std::cerr << "Warning: File '" << input_filename << "' passed as it has a not supported format!" << std::endl;
    }
}

// WARNING: do not list empty files!!!
void list_all_files_from(const std::string &dir_path, std::vector<std::string> *res);

#endif //COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H
