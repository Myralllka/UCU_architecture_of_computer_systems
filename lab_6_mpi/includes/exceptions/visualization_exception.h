//
// Created by solia on 5/31/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_EXECPTION_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_EXECPTION_H

#include <exception>
#include <string>
#include <utility>

class VisualizationException : public std::exception {
    std::string error_msg;
public:
    explicit VisualizationException(std::string  msg) : error_msg(std::move(msg)) {}

    [[nodiscard]] const char *what() const noexcept override {
        std::cerr << "Visualization error";
        return error_msg.data();
    }
};

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_EXECPTION_H
