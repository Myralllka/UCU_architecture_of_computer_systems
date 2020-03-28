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

static std::string extract_to_memory(std::string &buffer) {
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;
    off_t filesize;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
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
        r = archive_write_header(ext, entry);
        filesize = archive_entry_size(entry);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
            std::string output (filesize, char{});
            r = archive_read_data(a, &output[0], output.size());
            std::cout << output << std::endl;
            if (r < ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                exit(1);
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            exit(1);
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    exit(0);
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_EXTRACTOR_H
