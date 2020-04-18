//
// Created by fenix on 4/17/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_WORD_COUNT_H
#define COUNT_NUMBER_OF_ALL_WORDS_WORD_COUNT_H

#include <string>
#include <map>

void count_words(std::string tmp_str, std::map<std::string, size_t> *result_map);

void alternative_word_count(std::string element, std::map<std::string, size_t> *map_of_words);

#endif //COUNT_NUMBER_OF_ALL_WORDS_WORD_COUNT_H
