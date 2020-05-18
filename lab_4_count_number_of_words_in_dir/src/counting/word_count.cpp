//
// Created by fenix on 4/17/20.
//

#include "../../includes/counting/word_count.h"
#include <boost/locale.hpp>


template<class InputIt, class BinOp>
static void for_each_token(InputIt first, InputIt last,
                           BinOp binary_op) {
    while (first != last) {
        const auto pos = std::find_if(first, last, [](const unsigned &c) { return c != '-' && !isalpha(c); });
        binary_op(first, pos);
        if (pos == last) break;
        first = next(pos);
    }
}


void fast_count_words(const std::string &str, std::map<std::string, size_t> *result_map) {
    using namespace std;
    for_each_token(cbegin(str), cend(str),
                   [result_map](auto first, auto second) {
                       if (first != second) {
                           (*result_map)[string(first, second)] += 1;
                       }
                   });
}


void count_words(const std::string &&str, std::map<std::string, size_t> *result_map) {
    using namespace boost::locale::boundary;
    // Create mapping of str for token iterator using global locale.
    ssegment_index words_index(word, str.begin(), str.end());
    // Define a rule
    words_index.rule(word_any);
    // Print all "words" -- chunks of word boundary
    for (ssegment_index::iterator it = words_index.begin(), e = words_index.end(); it != e; ++it)
        (*result_map)[*it] += 1;;
}
