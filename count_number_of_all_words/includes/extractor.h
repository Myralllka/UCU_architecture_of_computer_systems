//
// Created by myralllka on 3/26/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_EXTRACTOR_H
#define COUNT_NUMBER_OF_ALL_WORDS_EXTRACTOR_H

#include <iostream>
#include <archive.h>
#include <archive_entry.h>


static int copy_data(struct archive *ar, struct archive *aw) {
    int r;
    const void *buff;
    size_t size;
    off_t offset;
    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(aw));
            return (r);
        }
    }
}

static void extract_to_memory(std::string& buffer, std::string output) {
    struct archive *a = archive_read_new();
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;
    size_t _used;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_memory(a, buffer.c_str(), buffer.size())))
        exit(1);
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF) break;
        if (r < ARCHIVE_OK) fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN) exit(1);
//        r = archive_write_open_memory(a, (void *) output.c_str(), output.size(), &_used);
        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK) fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
            r = copy_data(a, ext);
            if (r < ARCHIVE_OK) fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN) exit(1);
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK) fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN) exit(1);
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    exit(0);
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_EXTRACTOR_H
