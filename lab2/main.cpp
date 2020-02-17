#include <iostream>
#include <chrono>
#include <atomic>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

long long read_word_by_word() {
    std::vector<std::string> v;
    std::string word, first_word;
    std::ifstream f("lorem.txt");
    int counter = 1;
    if (f) std::cout << "Successfully open the file" << std::endl;
    auto stage1_start_time = get_current_time_fenced();
    f >> first_word;
    std::for_each(first_word.begin(), first_word.end(), [](auto &c) { c = ::tolower(c); });
    if (ispunct(first_word.back())) first_word.pop_back();
    while (f >> word) {
        std::for_each(word.begin(), word.end(), [](auto &c) { c = ::tolower(c); });
        if (ispunct(word.back())) word.pop_back();
        if (word == first_word) counter++;
    }
    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - stage1_start_time;
    std::cout << counter << std::endl;
    return to_us(total_time);
}

long long read_all_words_in_memory() {
    std::ifstream f("lorem.txt");
    std::string first_word, word;
    f >> first_word;
    std::for_each(first_word.begin(), first_word.end(), [](auto &c) { c = ::tolower(c); });
    if (ispunct(first_word.back())) first_word.pop_back();
    auto s = static_cast<std::ostringstream &>(
            std::ostringstream{} << f.rdbuf()).str();
    int counter = 1;
    auto stage1_start_time = get_current_time_fenced();
    for (auto &chr:s) {
        if (isalpha(chr)) word += tolower(chr);
        else if (isspace(chr)) {
            if (word == first_word) ++counter;
            word = "";
        }
    }
    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - stage1_start_time;
    std::cout << counter << std::endl;
    return to_us(total_time);
}

long long read_all_words_in_memory_count_using_boost_regex() {
    std::ifstream f("lorem.txt");
    std::string first_word, word, first_word_lowercase;
    f >> first_word;
    first_word_lowercase = first_word;
    std::string regex_line = "(";
    boost::algorithm::to_lower(first_word_lowercase);
    regex_line.append(first_word.substr(0, 1))
            .append("|")
            .append(first_word_lowercase.substr(0, 1))
            .append(")")
            .append(first_word.substr(1, first_word.length()))
            .append("?.");
    std::regex const expression(regex_line);
    std::for_each(first_word.begin(), first_word.end(), [](auto &c) { c = ::tolower(c); });
    if (ispunct(first_word.back())) first_word.pop_back();
    auto s = static_cast<std::ostringstream &>(std::ostringstream{} << f.rdbuf()).str();
    auto stage1_start_time = get_current_time_fenced();
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), expression);
    auto words_end = std::sregex_iterator();
    auto count = std::distance(words_begin, words_end) + 1;
    auto finish_time = get_current_time_fenced();
    auto total_time = finish_time - stage1_start_time;
    std::cout << "count: " << count << std::endl;
    return to_us(total_time);
};

int main() {
    long long word_by_word = read_word_by_word();
    std::cout << word_by_word << std::endl;
    long long all_words_in_memory = read_all_words_in_memory();
    std::cout << all_words_in_memory << std::endl;
    long long all_words_in_memory_count_using_boost_regex = read_all_words_in_memory_count_using_boost_regex();
    std::cout << all_words_in_memory_count_using_boost_regex << std::endl;
    return 0;
}
