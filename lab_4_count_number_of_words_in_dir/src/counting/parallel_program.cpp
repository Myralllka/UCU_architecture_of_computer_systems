//
// Created by solia on 4/5/20.
//

#include "../../includes/counting/parallel_program.h"
#include "../../includes/counting/map_helpers.h"
#include "../../includes/speed_tester.h"
#include "../../includes/counting/word_count.h"
#include <vector>
#include <thread>


#include "../../includes/debug_control.h"

//#define PACKET_SIZE 10000

// publish data for threads (POISON PILL == "" EMPTY STRING)

static void unarchive_thread(t_queue<file_packet> *file_q, tqueue_radio<std::string> *data_q) {
    data_q->publish();
    file_packet packet;

    while (!(packet = file_q->pop_front()).empty()) {
#ifdef DEBUG_INFO
        std::cout << "*" << std::flush;
#endif
        if (packet.archived) {
            archive_t tmp_archive{std::move(packet.content)};
            tmp_archive.extract_all(data_q);
        } else {
            data_q->emplace_back(std::move(packet.content));
        }
    }

    file_q->emplace_back(file_packet{});
    data_q->unpublish(); // IF LAST SEND A POISON PILL!!!
#ifdef DEBUG_INFO
    std::cout << "U" << std::flush;
#endif
}

static void count_thread(tqueue_radio<std::string> *data_q, tqueue_radio<std::map<std::string, size_t>> *map_queue) {
    map_queue->publish();
    data_q->subscribe();
    std::map<std::string, size_t> result_map{};
    std::string content;

    while (!(content = data_q->pop_front()).empty()) {
#ifdef DEBUG_INFO
        std::cout << "." << std::flush;
#endif
        fast_count_words(content, &result_map);
    }
    data_q->emplace_back("");
    data_q->unsubscribe();

    map_queue->unpublish(); // IF LAST SEND A POISON PILL!!!
    map_queue->emplace_back(std::move(result_map));
#ifdef DEBUG_INFO
    std::cout << "C" << std::flush;
#endif
}

static void merge_maps_thread(tqueue_radio<std::map<std::string, size_t>> *queue) {
    queue->subscribe();
    std::vector<std::map<std::string, size_t>> merge_group;
    while (!(merge_group = queue->pop_front_n(2))[0].empty() && !merge_group[1].empty()) {
#ifdef DEBUG_INFO
        std::cout << "+" << std::flush;
#endif
        for (auto &element : merge_group[1]) {
            merge_group[0][element.first] += merge_group[1][element.first];
        }
        queue->emplace_front(std::move(merge_group[0]));
    }
    if (merge_group[1].empty()) {
        queue->emplace_front(std::move((merge_group[0])));
    } else {
        queue->emplace_front(std::move(merge_group[1]));
    }
    queue->emplace_back(std::map<std::string, size_t>{});
    queue->unsubscribe();
#ifdef DEBUG_INFO
    std::cout << "M" << std::flush;
#endif
}

void parallel_count(t_queue<file_packet> *loader_queue,
                    const std::string &output_filename_a, const std::string &output_filename_n,
                    const uint8_t number_of_threads) {
    std::vector<std::thread> vector_of_threads{};

    tqueue_radio<std::string> data_queue;
    tqueue_radio<std::map<std::string, size_t>> map_queue;

//    size_t data_portion_len = PACKET_SIZE; // TODO: limit queue size
    auto analyzing_time = get_current_time_fenced();

    /////////////////////////// UNARCHIVE ///////////////////////////
    vector_of_threads.emplace_back(unarchive_thread, loader_queue, &data_queue);
    /////////////////////////////////////////////////////////////////

    ///////////////////////////   COUNT   ///////////////////////////
    for (uint8_t i = 0; i < number_of_threads; i++)
        vector_of_threads.emplace_back(count_thread, &data_queue, &map_queue);
    /////////////////////////////////////////////////////////////////

    merge_maps_thread(&map_queue);
    for (auto &t: vector_of_threads) {
        t.join();
    }

#ifdef DEBUG_INFO
    std::cout << "Analyzing: " << to_s(get_current_time_fenced() - analyzing_time) << std::endl;
#else
    std::cout << "Analyzing: " << to_us(get_current_time_fenced() - analyzing_time) << std::endl;
#endif

    dump_map_to_files(map_queue.pop_front(), output_filename_a, output_filename_n);
}
