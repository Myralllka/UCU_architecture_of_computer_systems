//
// Created by solia on 4/5/20.
//

#include "../../includes/merging/merge_maps.h"
#include "../../includes/speed_tester.h"
#include "../../includes/files/file_interface.h"
#include "../../includes/files/read_file.h"
#include "../../includes/queues/tqueue.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <boost/locale.hpp>
#include <thread>
#include <cmath>
#include "../../includes/counting/parallel_program.h"


#define PACKET_SIZE 10000

void count_words(const std::vector<std::string> &data, size_t start_position, size_t end_position, t_queue<std::map<std::string, int>> &res_map) {
    std::cout << &res_map[0] << std::endl;

    std::string word;

    std::map<std::string, int> thread_map = std::map<std::string, int>{};
    std::map<std::string, int> tmp{};
    result.emplace_back(std::move(tmp));
}


void parallel_count(const std::string &input_filename, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t number_of_threads) {
    std::vector<std::string> data;
    std::map<std::string, int> map_of_words;
    std::vector<std::thread> vector_of_threads{};
    t_queue<std::map<std::string, int>> result;
    std::vector<std::string> words;
    read_input_file(input_filename, data);
//        for (auto &str : data) {
    auto str = data[0];
    str = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(str)));
    str.erase(std::remove_if(str.begin(), str.end(), [](const unsigned &c) { return !isspace(c) && !isalpha(c); }),
              str.end());
    boost::split(words, str, boost::is_any_of("\t"));

    size_t data_portion_len = words.size() / number_of_threads;

    for (uint8_t i = 0; i < std::ceil(number_of_threads / 2); i++) {
        vector_of_threads.emplace_back(count_words, std::ref(words), i * data_portion_len, std::min((i + 1) * data_portion_len, words.size()), std::ref(result));
        count_words(words, i * data_portion_len, std::min((i + 1) * data_portion_len, words.size()), result);
    }

    for (auto &t: vector_of_threads) {
        t.join();
    }

    print(map_of_words, output_filename_a, output_filename_n);
}