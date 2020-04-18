//
// Created by fenix on 4/2/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_ARCHIVE_T_H
#define COUNT_NUMBER_OF_ALL_WORDS_ARCHIVE_T_H

#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <vector>
#include <ibase.h>
#include "../queues/tqueue.h"
#include "linear_extractor.h"
#include "../exceptions/not_implemented_err.h"

#include "../debug_control.h"

class archive_t {
    struct archive *archive_obj = nullptr;
    std::string buffer = "";
    uint32_t processed_f = 0;
    uint32_t failed_f = 0;

public:
    archive_t();

    explicit archive_t(std::string buffer);

    ~archive_t();

    archive_t(const archive_t &archive) = delete;

    const archive_t &operator=(const archive_t &archive) = delete;

    template<class T>
    static void extract_to(std::string file_name, T *tqueue);

    void load_file(const std::string &file_name);

    template<class T>
    void extract_all(T *tqueue);

private:
    void init_archive();
};


template<class T>
void archive_t::extract_all(T *tqueue) {
    struct archive_entry *entry;
    int status;
    la_int64_t filesize;

    // read from buffer, not from the file
    if ((status = archive_read_open_memory(archive_obj, buffer.c_str(), buffer.size()))) {
        std::cerr << "Error: Can not open archive_obj form memory! " << archive_error_string(archive_obj) << std::endl;
        return;
    }

    while ((status = archive_read_next_header(archive_obj, &entry)) != ARCHIVE_EOF && status != ARCHIVE_FATAL) {
        processed_f++;

        if (status < ARCHIVE_OK) {
            if (status < ARCHIVE_WARN) {
                std::cerr << "Error: Serious archive_obj damage! " << archive_error_string(archive_obj) << std::endl;
                failed_f++;
                continue;
            }
            std::cerr << "Warning: Some bad blocks in the archive_obj! " << archive_error_string(archive_obj)
                      << std::endl;
        }


        if (archive_entry_size(entry) > 0) {
#ifdef DEBUG_INFO
//            std::cout << "Unarchive entry: " << "(size: " << archive_entry_size(entry) << ")" << std::endl;
            std::cout << "e" << std::flush;
#endif
            filesize = archive_entry_size(entry);
            std::string output(filesize, char{});

            // write exactly to the other output buffer
            status = archive_read_data(archive_obj, &output[0], output.size());
            if (status >= ARCHIVE_WARN) {
                tqueue->emplace_back(std::move(output));
            } else {
                std::cerr << archive_error_string(archive_obj) << std::endl;
            }
        }
    }

    if (status == ARCHIVE_FATAL) {
        failed_f++;
        std::cerr << "Error: Fatal archive_obj damage! " << archive_error_string(archive_obj) << std::endl;
    }
}

template<class T>
void archive_t::extract_to(std::string file_name, T *tqueue) {
    archive_t tmp_archive{};
    tmp_archive.load_file(file_name);
    tmp_archive.extract_all(tqueue);
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_ARCHIVE_T_H
