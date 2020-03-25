//
// Created by myralllka on 3/25/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_PARSER_EXEPTION_H
#define COUNT_NUMBER_OF_ALL_WORDS_PARSER_EXEPTION_H

#include <exception>

class OptionsParseException : public std::exception {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "Invalid configuration file or content!";
    }
};


#endif //COUNT_NUMBER_OF_ALL_WORDS_PARSER_EXEPTION_H
