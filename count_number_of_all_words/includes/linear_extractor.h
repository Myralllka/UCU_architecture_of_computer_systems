//
// Created by myralllka on 3/26/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_LINEAR_EXTRACTOR_H
#define COUNT_NUMBER_OF_ALL_WORDS_LINEAR_EXTRACTOR_H

#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <vector>
//#include <boost/coroutine2/all.hpp>
#include "tqueue.h"


template<class T>
// The base of the code is from https://github.com/libarchive/libarchive/wiki/Examples#a-universal-decompressor
static void extract_to_memory(const std::string &buffer, T *result) {
    struct archive *a;
    struct archive_entry *entry;
    int status = ARCHIVE_OK;
    off_t filesize;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);

    // read from buffer, not from the file
    if (archive_read_open_memory(a, buffer.c_str(), buffer.size())) {
        std::cerr << "Error: Can not open archive form memory!" << archive_error_string(a) << std::endl;
        archive_read_close(a); // TODO: test if this line is needed
        archive_read_free(a);
        return;
    }

    while (status > ARCHIVE_FATAL) {
        status = archive_read_next_header(a, &entry);
        if (status == ARCHIVE_EOF) {
            break;
        } else if (status < ARCHIVE_OK) {
            if (status < ARCHIVE_WARN) {
                std::cerr << "Error: Serious archive damage!" << archive_error_string(a) << std::endl;
                continue;  // TODO: test for critical status
            }
            std::cerr << "Warning: Some bad blocks in the archive!" << archive_error_string(a) << std::endl;
        }

        if ((filesize = archive_entry_size(entry)) == 0) {
            continue;
        }
        std::string output(filesize, char{});  // TODO: probably should be also generic

        // write exactly to the other output buffer
        status = archive_read_data(a, &output[0], output.size());
        if (status >= ARCHIVE_WARN) {
            result->emplace_back(std::move(output));
        } else {
            std::cerr << "Error: Serious archive damage!" << archive_error_string(a) << std::endl;
        }
    }
    archive_read_close(a);
    archive_read_free(a);
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_LINEAR_EXTRACTOR_H
