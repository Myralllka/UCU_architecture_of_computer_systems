//
// Created by myralllka on 3/26/20.
// Code from https://github.com/libarchive/libarchive/wiki/Examples#A_Universal_Decompressor
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_EXTRACTOR_H
#define COUNT_NUMBER_OF_ALL_WORDS_EXTRACTOR_H

#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <vector>
#include <boost/coroutine2/all.hpp>

static std::vector<std::string> extract_to_memory(std::string &buffer) {
    struct archive *a;
    struct archive_entry *entry;
    int r;
    std::vector<std::string> result;
    off_t filesize;
    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    if ((r = archive_read_open_memory(a, buffer.c_str(), buffer.size())))
        exit(1);
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            exit(1);
        filesize = archive_entry_size(entry);
        std::string output(filesize, char{});
        r = archive_read_data(a, &output[0], output.size());
//        std::cout << output << std::endl;
        result.push_back(output);
        if (r < ARCHIVE_WARN)
            exit(1);
    }
    archive_read_close(a);
    archive_read_free(a);
    return result;
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_EXTRACTOR_H
