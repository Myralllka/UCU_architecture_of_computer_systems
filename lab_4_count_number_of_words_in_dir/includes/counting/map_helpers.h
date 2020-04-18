//
// Created by fenix on 4/6/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_MAP_HELPERS_H
#define COUNT_NUMBER_OF_ALL_WORDS_MAP_HELPERS_H

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

inline void print_map(const std::map<std::string, int> &map) {
    for (auto &pair:map) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
}

#endif //COUNT_NUMBER_OF_ALL_WORDS_MAP_HELPERS_H
