//
// Created by fenix on 4/2/20.
//

#include <fstream>
#include <sstream>
#include "../includes/archive_t.h"

archive_t::archive_t(const std::string *buffer) : buffer(buffer) {
    init_archive();
}

archive_t::archive_t() {
    init_archive();
}

archive_t::~archive_t() {
    if (BUFFER_IS_ALLOCATED) {
        delete buffer;
    }
    archive_read_close(archive); // TODO: archive can be not open
    archive_read_free(archive);
}

void archive_t::init_archive() {
    archive = archive_read_new();
    archive_read_support_format_all(archive);
    archive_read_support_filter_all(archive);
}

void archive_t::load_file(const std::string &file_name) {
    std::ifstream raw_file(file_name, std::ios::binary);

    std::ostringstream ss{};
    ss << raw_file.rdbuf();

    buffer = new std::string{ss.str()};
    BUFFER_IS_ALLOCATED = true;
}




