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
#include "tqueue.h"
#include "linear_extractor.h"
#include "not_implemented_err.h"

class archive_t {
    struct archive *archive = nullptr;
//    t_queue<struct archive_entry *> entry_qt{};
    const std::string *buffer = nullptr;
    bool BUFFER_IS_ALLOCATED = false;

    // processed files statistics
//    uint32_t processed_f = 0;
//    uint32_t failed_f = 0;

public:
    archive_t();

    explicit archive_t(const std::string *buffer);

    ~archive_t();

    archive_t(const archive_t &archive) = delete;

    const archive_t &operator=(const archive_t &archive) = delete;

    void load_file(const std::string &file_name);

    template<class T>
    // TODO: unimplemented (need to pass by element of archive)
    void extract_files_t(T *tqueue, uint16_t threads_n = 1);

private:
    void init_archive();

    // archive should be open !!!
    template<class T>
    // TODO: unimplemented (need to pass by element of archive)
    static void generic_extract_files(T *vector, t_queue<archive_entry *> *source_qt);
};

// archive should be open !!!
template<class T>
void archive_t::generic_extract_files(T *vector, t_queue<struct archive_entry *> *source_qt) {
    throw NotImplementedException{};  // TODO: unimplemented (need to pass by element of archive)
    /*
    struct archive_entry * entry = source_qt->pop_front();
//    off_t filesize;
    la_int64_t filesize;
    int status;
    while (entry != nullptr) {
        filesize = archive_entry_size(entry);
        std::string output(filesize, char{});

        // write exactly to the other output buffer
        status = archive_read_data(archive, &output[0], output.size());
        if (status >= ARCHIVE_WARN) {
            vector->emplace_back(std::move(output));
        } else {
//            std::cerr << archive_error_string(archive) << std::endl;
        }
    }
     */
}

template<class T>
void archive_t::extract_files_t(T *tqueue, uint16_t threads_n) {
    if (threads_n == 1) {
        extract_to_memory<T>(*buffer, tqueue);
        return;
    }
    throw NotImplementedException{}; // TODO: unimplemented (need to pass by element of archive)
    /*
    struct archive_entry * entry;
    int status;
    std::vector<std::thread> threads_v(threads_n);
    for (int i = 0; i < threads_n; ++i) {
        threads_v.emplace_back(generic_extract_files<T>, tqueue); // TODO: unimplemented (need to pass by element of archive)
    }

    while ((status = archive_read_next_header(archive, &entry)) != ARCHIVE_EOF && status != ARCHIVE_FATAL) {
        processed_f++;
        if (status < ARCHIVE_OK) {
            if (status < ARCHIVE_WARN) {
                std::cerr << "Error: Serious archive damage!" << archive_error_string(archive) << std::endl;
                failed_f++;
                continue;
            }
            std::cerr << "Warning: Some bad blocks in the archive!" << archive_error_string(archive) << std::endl;
        }
        if (archive_entry_size(entry) > 0) {
            entry_qt.push(entry);
        }
    }

    if (status == ARCHIVE_FATAL) {
        failed_f++;
        std::cerr << "Error: Fatal archive damage!" << archive_error_string(archive) << std::endl;
    }
     */
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_ARCHIVE_T_H
