//
// Created by fenix on 4/17/20.
//

#include <boost/locale.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "../../includes/counting/word_count.h"


void count_words(std::string tmp_str, std::map<std::string, size_t> *result_map) {
    std::vector<std::string> words;
    tmp_str = boost::locale::to_lower(
            boost::locale::fold_case(boost::locale::normalize(tmp_str))); // added and not tested
    boost::split(words, tmp_str, boost::is_any_of("\t\n "));

    for (auto &elem : words) {
        elem.erase(std::remove_if(elem.begin(), elem.end(),
                                  [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), elem.end());
        if (elem.empty() || std::all_of(elem.begin(), elem.end(), isspace)) continue;
        (*result_map)[elem] += 1; // TODO: check if it is needed to initialize new keys
    }
}


void alternative_word_count(std::string element, std::map<std::string, size_t> *map_of_words) {
    std::string word;

    element = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(element)));
    element.erase(std::remove_if(element.begin(), element.end(),
                                 [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), element.end());
    for (auto &chr : element) {
        if (isalpha(chr))
            word += tolower(chr);
        else if (isspace(chr) && !word.empty()) {
            auto itr = map_of_words->find(word);
            if (itr != map_of_words->end()) {
                (*map_of_words)[word] += 1;
            } else {
                (*map_of_words)[word] = 1;
            }
            word.clear();
        }
    }
}
