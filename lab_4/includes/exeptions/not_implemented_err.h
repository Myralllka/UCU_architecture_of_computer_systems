//
// Created by fenix on 4/5/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_NOT_IMPLEMENTED_ERR_H
#define COUNT_NUMBER_OF_ALL_WORDS_NOT_IMPLEMENTED_ERR_H

#include <exception>

class NotImplementedException : public std::exception {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "Invalid configuration file or content!";
    }
};

#endif //COUNT_NUMBER_OF_ALL_WORDS_NOT_IMPLEMENTED_ERR_H
