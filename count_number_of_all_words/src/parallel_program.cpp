//
// Created by solia on 4/5/20.
//

#include "../includes/parallel_program.h"
#include "../includes/linear_program.h"
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

#define PACKET_SIZE 1000

void count_words(std::string &data, const size_t start_position, const size_t end_position, t_queue<std::map<std::string, int>> &queue) {
    size_t start = start_position;
    size_t end = end_position;
    if (start_position - 1 > 0 && !isspace(data[start_position - 1])) {
        while (!isspace(data[start - 1])) {
            start--;
        }
    }
    while (!isspace(data[end - 1])) {
        end--;
    }
    std::string word;


    std::map<std::string, int> thread_map = std::map<std::string, int>{};
    for (auto &chr : data.substr(start_position, end_position)) {
            if (isalpha(chr))
                word += tolower(chr);
            else if (isspace(chr)) {
                auto itr = thread_map.find(word);
                if (itr != thread_map.end()) {
                    thread_map[word] += 1;
                } else {
                    thread_map[word] = 1;
                }
                word.clear();

                if (thread_map.size() > PACKET_SIZE) {
                    queue.emplace_back(std::move(thread_map));
                    thread_map.clear();
                }
            }
        }
    queue.emplace_back(std::move(thread_map));
    queue.emplace_back(std::map<std::string, int>{});
}

void merge_maps_queue(t_queue<std::map<std::string, int>> &queue) {
    std::map<std::string, int> tmp_map;
    std::map<std::string, int> thread_map{};
    std::cout << 2 << std::endl;
    while (!(tmp_map = queue.pop_front()).empty()) {
        thread_map.insert(tmp_map.begin(), tmp_map.end());
    }
    std::cout << 3 << std::endl;
    queue.emplace_back(std::move(thread_map));
    queue.emplace_back(std::map<std::string, int>{});
}

void parallel_count(const std::string &input_filename, const std::string &output_filename_a,
                    const std::string &output_filename_n, const uint8_t num_threads) {
    // read entire binary archive into the buffer
    auto total_time = get_current_time_fenced();
    std::ifstream raw_file(input_filename, std::ios::binary);
    std::vector<std::string> data;

    // read buffer
    if (input_filename.substr(input_filename.find_last_of('.') + 1) == "txt") {
        std::ifstream f(input_filename);
        std::stringstream s{};
        s << f.rdbuf();
        data.emplace_back(s.str());
    } else {
        extract_to_memory(read_binary_file_into_buffer(input_filename), &data);
    }
    std::string file_data = data[0];

    namespace bl =  boost::locale;

    file_data = bl::to_lower(bl::fold_case(bl::normalize(file_data)));
    file_data.erase(std::remove_if(file_data.begin(), file_data.end(),
                                 [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), file_data.end());

    auto finish_time = get_current_time_fenced();
    auto analyzing_time = get_current_time_fenced();
    size_t data_portion_len = std::ceil(file_data.size() / num_threads * 2);

    // parallel counting
    std::vector<std::thread> vector_of_threads;
    t_queue<std::map<std::string, int>> queue_of_maps;

    for (uint8_t i = 0; i < std::ceil(num_threads / 2); i++) {
        vector_of_threads.emplace_back(count_words, std::ref(file_data), i * data_portion_len,
                                       std::min((i + 1) * data_portion_len, data.size()), std::ref(queue_of_maps));
    }

    for (uint8_t i = std::ceil(num_threads / 2); i < num_threads; i++) {
        vector_of_threads.emplace_back(merge_maps_queue, std::ref(queue_of_maps));
    }

    for (auto &t: vector_of_threads) {
        t.join();
    }
    std::cout << 1 << std::endl;
    std::map<std::string, int> map_of_all_words = queue_of_maps.pop_front();

//    std::map<std::string, int> map_of_all_words = queue_of_maps.pop_back();
    std::cout << "Loading: " << to_us(finish_time - total_time) << std::endl;
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
    print(map_of_all_words, output_filename_a, output_filename_n);
}
