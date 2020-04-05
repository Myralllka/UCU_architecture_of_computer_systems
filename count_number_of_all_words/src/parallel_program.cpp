//
// Created by solia on 4/5/20.
//

#include "../includes/parallel_program.h"
#include "../includes/linear_extractor.h"
#include "../includes/merge_maps.h"
#include "../includes/print_maps_to_files.h"
#include "../includes/tqueue.h"
#include "../includes/speed_tester.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <boost/locale.hpp>
#include <thread>
#include <cmath>


void count_words(std::string &data, const int start_position, const int end_position,
                 std::map<std::string, int> &map_of_words, t_queue<std::map<std::string, int>> &queue) {
    int start = start_position;
    int end = end_position;
    if (start_position - 1 > 0 && !isspace(data[start_position - 1])) {
        while (!isspace(data[start_position - 1])) {
            start--;
        }
    }
    while (!isspace(data[end_position - 1])) {
        end--;
    }
    std::string word;
    for (auto &chr : data.substr(start_position, end_position)) {
            if (isalpha(chr))
                word += tolower(chr);
            else if (isspace(chr)) {
                auto itr = map_of_words.find(word);
                if (itr != map_of_words.end()) {
                    map_of_words[word] += 1;
                } else {
                    map_of_words[word] = 1;
                }
                word.clear();
            }
        }
//    queue.emplace_back(merge_maps(map_of_words, map_of_words));
}

void merge_maps_queue(t_queue<std::map<std::string, int>> &queue) {
    if (queue.get_size() > 1) {
        std::map<std::string, int> map1 = queue.pop_front();
        std::map<std::string, int> map2 = queue.pop_front();
        queue.emplace_back(merge_maps(map1, map2));
    }
}

void parallel_count(const std::string &input_filename, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t num_threads) {
    // read entire binary archive into the buffer
    auto total_time = get_current_time_fenced();
    std::ifstream raw_file(input_filename, std::ios::binary);
    std::vector<std::string> data;

    // read buffer
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    extract_to_memory(buffer, &data);
    std::string file_data = data[0];

    namespace bl =  boost::locale;

    file_data = bl::to_lower(bl::fold_case(bl::normalize(file_data)));
    file_data.erase(std::remove_if(file_data.begin(), file_data.end(),
                                 [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), file_data.end());

    auto finish_time = get_current_time_fenced();
    auto analyzing_time = get_current_time_fenced();
    size_t data_portion_len = std::ceil(file_data.size() / num_threads * 2);

    // parallel counting
    std::vector<std::map<std::string, int>> vector_of_maps;
    t_queue<std::map<std::string, int>> queue_of_maps;
    std::vector<std::thread> vector_of_threads(num_threads);
    for (uint8_t i = 0; i < std::ceil(num_threads / 2); i++) {
        std::map<std::string, int> map;
        vector_of_maps.push_back(map);

        vector_of_threads.emplace_back(count_words, std::ref(file_data), i * data_portion_len,
                                       std::min((i + 1) * data_portion_len, data.size()),
                                       std::ref(vector_of_maps[i]), std::ref(queue_of_maps));
    }

//    for (uint8_t i = std::floor(num_threads / 2); i < num_threads; i++) {
//        vector_of_threads.emplace_back(merge_maps_queue, std::ref(queue_of_maps));
//    }
    std::map<std::string, int> map_of_all_words;
    for (auto &map: vector_of_maps) {
        map_of_all_words = merge_maps(map_of_all_words, map);
    }
    for (auto &t: vector_of_threads) {
        t.join();
    }

//    std::map<std::string, int> map_of_all_words = queue_of_maps.pop_back();
    std::cout << "Loading: " << to_us(finish_time - total_time) << std::endl;
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
    print(map_of_all_words, output_filename_a, output_filename_n);
}
