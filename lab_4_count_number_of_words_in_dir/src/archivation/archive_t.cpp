//
// Created by fenix on 4/2/20.
//

#include <fstream>
#include <sstream>
#include <utility>
#include "../../includes/archivation/archive_t.h"
#include "../../includes/files/file_interface.h"

archive_t::archive_t(std::string buffer) : buffer(std::move(buffer)) {
    init_archive();
}

archive_t::archive_t() {
    init_archive();
}

archive_t::~archive_t() {
    archive_read_close(archive_obj); // TODO: archive_obj can be not open
    archive_read_free(archive_obj);
}

void archive_t::init_archive() {
    archive_obj = archive_read_new();
    archive_read_support_format_all(archive_obj);
    archive_read_support_filter_all(archive_obj);
}

void archive_t::load_file(const std::string &file_name) {
    buffer = read_binary_file(file_name);
}
