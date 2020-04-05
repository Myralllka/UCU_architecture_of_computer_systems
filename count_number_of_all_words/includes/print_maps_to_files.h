//
// Created by myralllka on 4/5/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_PRINT_MAPS_TO_FILES_H
#define COUNT_NUMBER_OF_ALL_WORDS_PRINT_MAPS_TO_FILES_H

#include <map>
#include <algorithm>

void print(const std::map<std::string, int> &map_of_words, const std::string &output_filename_a,
           const std::string &output_filename_n);

template<typename A, typename B>
std::pair<B, A> flip_pair(const std::pair<A, B> &p) {
    return std::pair<B, A>(p.second, p.first);
}

template<typename A, typename B, template<class, class, class...> class M, class... Args>
std::multimap<B, A> flip_map(const M<A, B, Args...> &src) {
    std::multimap<B, A> dst;
    std::transform(src.begin(), src.end(),
                   std::inserter(dst, dst.begin()),
                   flip_pair<A, B>);
    return dst;
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_PRINT_MAPS_TO_FILES_H
