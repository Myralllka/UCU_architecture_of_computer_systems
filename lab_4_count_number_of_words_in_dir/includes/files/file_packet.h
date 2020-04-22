//
// Created by fenix on 4/22/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_FILE_PACKET_H
#define COUNT_NUMBER_OF_ALL_WORDS_FILE_PACKET_H

#include <string>

struct file_packet {
    bool archived;
    std::string content;

    explicit file_packet(std::string content = "", bool compressed = false) : archived(compressed),
                                                                              content(std::move(content)) {}

    [[nodiscard]] bool empty() const { return content.empty(); }
};

#endif //COUNT_NUMBER_OF_ALL_WORDS_FILE_PACKET_H
