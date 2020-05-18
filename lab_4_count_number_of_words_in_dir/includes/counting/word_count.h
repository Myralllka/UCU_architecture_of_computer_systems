//
// Created by fenix on 4/17/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_WORD_COUNT_H
#define COUNT_NUMBER_OF_ALL_WORDS_WORD_COUNT_H

#include <string>
#include <map>

void fast_count_words(const std::string &str, std::map<std::string, size_t> *result_map);

void count_words(const std::string &&str, std::map<std::string, size_t> *result_map);

#endif //COUNT_NUMBER_OF_ALL_WORDS_WORD_COUNT_H
