//
// Created by solia on 5/31/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_EXECPTION_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_EXECPTION_H

#include <exception>
#include <string>

class VisualizationException : public std::exception {
    std::string error_msg;
public:
    VisualizationException(const std::string& msg) : error_msg(msg) {}

    [[nodiscard]] const char *what() const noexcept override {
        return "Visualization error: " + error_msg + "!";
    }
};

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_VISUALIZATION_EXECPTION_H
