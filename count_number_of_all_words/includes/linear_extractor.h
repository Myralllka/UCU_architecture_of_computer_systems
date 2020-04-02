//
// Created by myralllka on 3/26/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_LINEAR_EXTRACTOR_H
#define COUNT_NUMBER_OF_ALL_WORDS_LINEAR_EXTRACTOR_H

#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <vector>
#include <boost/coroutine2/all.hpp>
#include "tqueue.h"


// The base of the code is from https://github.com/libarchive/libarchive/wiki/Examples#a-complete-extractor
static std::vector<std::string> extract_to_memory(const std::string &buffer) {
    struct archive *a;
    struct archive_entry *entry;
    int status;
    std::vector<std::string> result;
    off_t filesize;
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    // read from buffer, not from the file
    if ((status = archive_read_open_memory(a, buffer.c_str(), buffer.size()))) {
        exit(1); // TODO: except errors
    }
    for (;;) {
        status = archive_read_next_header(a, &entry);
        if (status == ARCHIVE_EOF) {
            break;
        }
        if (status < ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(a));
        }
        if (status < ARCHIVE_WARN) {
            exit(1); // TODO: except errors
        }
        filesize = archive_entry_size(entry);
        std::string output(filesize, char{});
        // write exactly to the other output buffer
        status = archive_read_data(a, &output[0], output.size());
//        result.push_back(output);
        result.emplace_back(std::move(output));
        if (status < ARCHIVE_WARN)
            exit(1);
    }
    archive_read_close(a);
    archive_read_free(a);
    return result;
}

// The base of the code is from https://github.com/libarchive/libarchive/wiki/Examples#A_Universal_Decompressor
static std::vector<std::string> extract_to_queue(const std::string &buffer, t_queue<std::string> *tqueue) {
    struct archive *a;
    struct archive_entry *entry;
    int status;
    off_t filesize;
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    // read from buffer, not from the file
    if ((status = archive_read_open_memory(a, buffer.c_str(), buffer.size())))
        exit(1);
    for (;;) {
        status = archive_read_next_header(a, &entry);
        if (status == ARCHIVE_EOF) {
            break;
        }
        if (status < ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(a));
        }
        if (status < ARCHIVE_WARN) {
            exit(1); // TODO: except errors
        }
        filesize = archive_entry_size(entry);
        std::string output(filesize, char{});
        //write explicitly to the other buffer
        status = archive_read_data(a, &output[0], output.size());
        tqueue->emplace_back(std::move(output)); // TODO: reduce copy of 'output'
        if (status < ARCHIVE_WARN) {
            exit(1); // TODO: except errors
        }
    }
    archive_read_close(a);
    archive_read_free(a);
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_LINEAR_EXTRACTOR_H
