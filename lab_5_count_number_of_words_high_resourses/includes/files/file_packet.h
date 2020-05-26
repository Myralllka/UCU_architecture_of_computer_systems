//
// Created by fenix on 4/22/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_FILE_PACKET_H
#define COUNT_NUMBER_OF_ALL_WORDS_FILE_PACKET_H

#include <string>

struct file_packet {
    bool archived;
    std::string content;

    explicit file_packet(std::string &&content = "", bool compressed = false)
            : archived(compressed), content(std::move(content)) {}

    explicit file_packet(file_packet &&other)
            : archived(other.archived), content(std::move(other.content)) {}

    explicit file_packet(const file_packet &other) = delete;

    file_packet &operator=(file_packet &&packet) {
        if (this != &packet) {
            this->content = std::move(packet.content);
            this->archived = packet.archived;
        }
        return *this;
    }

    [[nodiscard]] bool empty() const { return content.empty(); }
};

#endif //COUNT_NUMBER_OF_ALL_WORDS_FILE_PACKET_H
