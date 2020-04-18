//
// Created by solia on 4/5/20.
//

#include "../../includes/counting/parallel_program.h"
#include "../../includes/merging/merge_maps.h"
#include "../../includes/speed_tester.h"
#include "../../includes/counting/word_count.h"
#include "../../includes/files/file_interface.h"
#include <vector>
#include <thread>

#include "../../includes/debug_control.h"

//#define PACKET_SIZE 10000

static void count_thread(t_queue<file_packet> *data_q, t_queue<std::map<std::string, size_t>> *map_queue) {
    std::map<std::string, size_t> result_map{};
//    auto analyzing_time = get_current_time_fenced();
    std::string content;
    std::vector<std::string> words;

    content = data_q->pop_front().use_as_one(data_q);
    while (!(content = data_q->pop_front().use_as_one(data_q)).empty()) {
#ifdef DEBUG_INFO
        std::cout << "." << std::flush;
#endif
        count_words(std::move(content), &result_map);
    }

//    std::cout << "j: " << to_us(get_current_time_fenced() - analyzing_time) << "\tsize: " << data_q.size() <<std::endl;
    data_q->emplace_back(file_packet{});
    map_queue->emplace_back(std::move(result_map));
    map_queue->emplace_back(std::map<std::string, size_t>{}); // POISON PILL
}

void parallel_count(t_queue<file_packet> &data_queue,
                    const std::string &output_filename_a, const std::string &output_filename_n,
                    const uint8_t number_of_threads) {
    std::vector<std::thread> vector_of_threads{};
    std::map<std::string, size_t> result_map;
    t_queue<std::map<std::string, size_t>> map_queue;
    std::vector<std::string> words;

//    size_t data_portion_len = PACKET_SIZE;
    auto analyzing_time = get_current_time_fenced();
    for (size_t i = 0; i < number_of_threads - 1; i++) {
        vector_of_threads.emplace_back(count_thread, &data_queue, &map_queue);
    }

    merge_maps_thread(&map_queue);
    for (auto &t: vector_of_threads) {
        t.join();
    }
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
    print(map_queue.pop_front(), output_filename_a, output_filename_n);
}
