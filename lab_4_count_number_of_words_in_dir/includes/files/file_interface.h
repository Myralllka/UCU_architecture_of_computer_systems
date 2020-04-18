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

void print(const std::map<std::string, size_t> &map_of_words, const std::string &output_filename_a,
           const std::string &output_filename_n);

std::string read_binary_file(const std::string &filename);

static inline std::string get_extension(const std::string &filename) {
    return filename.substr(filename.find_last_of('.') + 1);
}


static inline bool is_text_file(std::string filename) {
    return "txt" == get_extension(filename);
}

static bool is_archive_file(const std::string &filename) {
    static const std::vector<std::string> supported_formats = {"tar", "gz", "pax", "cpio", "zip", "mtree", "shar", "ar",
                                                               "raw", "xar", "lha", "lzh", "rar", "7z"};
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

    template<class T>
    std::string use_as_one(T *data_q) {
        if (!archived) {
            return std::move(content);
        }
        archive_t archive{std::move(content)};
        archive.extract_all(data_q);
        return data_q->pop_front().content;
    }

    bool empty() const { return content.empty(); }
};

template<class T>
void read_input_file(const std::string &input_filename, T *data_struct) {

#ifdef DEBUG_INFO
    std::cout << "load file: " << input_filename << std::endl;
#endif

    if (is_text_file(input_filename)) {
        data_struct->emplace_back(file_packet{read_binary_file(input_filename)});
    } else if (is_archive_file(input_filename)) {
        data_struct->emplace_back(file_packet{read_binary_file(input_filename), true});
//        archive_t::extract_to(input_filename, data_struct);
    } else {
        std::cerr << "Warning: File '" << input_filename << "' passed as it has a not supported format!" << std::endl;
    }
}

// publish data for threads (POISON PILL == "" EMPTY STRING)
template<class S, class T>
void read_files_to(const S &file_list, T *data_struct) {
    for (const auto &file_name : file_list) {
        read_input_file(file_name, data_struct);
    }
    data_struct->push_back(file_packet{});
}

// WARNING: do not list empty files!!!
inline std::vector<std::string> list_all_files_from(const std::string &dir_path) {
    std::vector<std::string> res{};
    for (const auto &file : boost::filesystem::recursive_directory_iterator(dir_path)) {
        if (boost::filesystem::is_regular_file(file) && !boost::filesystem::is_empty(file)) {
            res.emplace_back(std::move(file.path().string()));
        }
    }
    return res;
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_FILE_INTERFACE_H
