//
// Created by solia on 4/5/20.
//

#include "../../includes/merging/merge_maps.h"
#include "../../includes/speed_tester.h"
#include "../../includes/files/file_interface.h"
#include "../../includes/queues/tqueue.h"
#include "../../includes/counting/parallel_program.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <boost/locale.hpp>
#include <thread>

#define PACKET_SIZE 10000

//inline void print_map(const std::map<std::string, int>& map) {
//    for (auto &pair:map) {
//        std::cout << pair.first << "\t" << pair.second << std::endl;
//    }
//}

void count(const std::vector<std::string> &data, t_queue<std::map<std::string, size_t>> &result_queue) {
    std::map<std::string, size_t> result_map{};
//    auto analyzing_time = get_current_time_fenced();
    for (auto element:data) {
        element.erase(std::remove_if(element.begin(), element.end(),
                                     [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), element.end());
        if (element.empty() || std::all_of(element.begin(), element.end(), isspace)) continue;
        result_map[boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(element)))] += 1;
    }
//    std::cout << "j: " << to_us(get_current_time_fenced() - analyzing_time) << "\tsize: " << data.size() <<std::endl;
    result_queue.emplace_back(std::move(result_map));
}

void parallel_count(const std::vector<std::string> &data, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t number_of_threads) {
//    std::vector<std::string> data;
    std::map<std::string, size_t> map_of_words;
    std::vector<std::thread> vector_of_threads{};
    std::vector<std::map<std::string, size_t>> vector_of_results{};
    t_queue<std::map<std::string, size_t >> result_queue;
    std::vector<std::string> words;
//        for (auto &str : data) {
    auto str = data[0];
//    str = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(str)));
//    str.erase(std::remove_if(str.begin(), str.end(), [](const unsigned &c) { return !isspace(c) && !isalpha(c); }),
//              str.end());
    boost::split(words, str, boost::is_any_of("\t\n "));
    size_t data_portion_len = (words.size() % number_of_threads) ? words.size() / number_of_threads + 1 : words.size() /
                                                                                                          number_of_threads;
//    size_t data_portion_len = PACKET_SIZE;
    auto analyzing_time = get_current_time_fenced();
    for (size_t i = 0; i < number_of_threads; i++) {
        std::vector<std::string> tmp;
        auto it = std::next(words.begin() + data_portion_len * i,
                            std::min(words.size() - data_portion_len * (i), data_portion_len));
        std::move(words.begin() + data_portion_len * i, it, std::back_inserter(tmp));
        vector_of_threads.emplace_back(count, std::move(tmp), std::ref(result_queue));
//        std::map<std::string, int> tmp_map;
//        vector_of_threads.emplace_back(count, std::move(tmp), std::move(tmp_map));
//        vector_of_results.emplace_back(std::ref(tmp_map));
    }
    for (auto &t: vector_of_threads) {
        t.join();
    }
    merge_maps_queue(result_queue, number_of_threads);
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
    print(result_queue.pop_back(), output_filename_a, output_filename_n);
}
