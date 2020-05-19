//
// Created by solia on 4/5/20.
//

#include "../../includes/counting/parallel_program.h"
#include "../../includes/counting/word_count.h"
#include <vector>
#include <thread>
#include <boost/locale.hpp>
#include "../../includes/archivation/archive_t.h"
#include "../../includes/code_control.h"

#ifdef VISUAL_PIPELINE
#include "../../includes/counting/visualization.h"
#include <thread>
#include "../../includes/queues/tqueue_radio.h"
#include "../../includes/queues/tqueue.h"
#include <map>
#include <string>
#include "../../includes/files/file_packet.h"
#include <cinttypes>


static void visual_pipeline(volatile bool *cond,
                            t_queue<file_packet> *file_q,
                            tqueue_radio<std::string> *data_q,
                            tqueue_radio<std::map<std::string, size_t>> *map_queue) {
    while (*cond) {
        printProgress(static_cast<double>(file_q->get_size()) / file_q->get_max_size());
        printProgressContinue(static_cast<double>(data_q->get_size()) / data_q->get_max_size());
        printProgressContinue(static_cast<double>(map_queue->get_size()) / map_queue->get_max_size());
        sleep(1);
    }
    std::cout << "END VISUAL" << std::endl;
}
#endif

#ifdef PROGRESS_BAR
#include "../../includes/counting/visualization.h"
#include <thread>
#include <iostream>
#include <atomic>
#endif

#ifdef DEBUG_INFO
#include <iostream>
#endif

#define MERGE_THREADS 2

#define MAX_DATA_QUEUE_SIZE_PER_THREAD 3
#define MAX_LOAD_QUEUE_SIZE 10

namespace ba = boost::locale::boundary;


static void read_files_thread(const std::vector<std::string> &file_list, t_queue<file_packet> *data_struct) {
#ifdef PROGRESS_BAR
    std::atomic_uint64_t caunt{0};
    volatile bool print_progress = true;
    std::thread visual_t{[](const volatile bool *caunt, std::atomic_uint64_t *num, size_t max_num) {
        while (*caunt) {
            printProgress(static_cast<double>(num->load()) / max_num);
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1.0s);
        }
    }, &print_progress, &caunt, file_list.size()};
#endif

    for (const auto &file_name : file_list) {
        read_input_file_gen(file_name, data_struct);

#ifdef PROGRESS_BAR
        caunt++;
#endif
    }
    data_struct->emplace_back(file_packet{});

#ifdef PROGRESS_BAR
    print_progress = false;
    visual_t.join();
#endif
}

static void count_thread(t_queue<file_packet> *file_q,
                         tqueue_radio<std::map<std::string, size_t>> *map_q) {
    map_q->publish();
    file_packet packet;
    std::deque<std::string> data_q;
    std::map<std::string, size_t> map_of_words{};
    std::string tmp_content;

    while (!(packet = file_q->pop_front()).empty()) {
        //////////////// LOAD FILE ///////////////////////////
        if (packet.archived) {
            archive_t tmp_archive{std::move(packet.content)};
            tmp_archive.extract_all(&data_q);
        } else {
            data_q.emplace_back(std::move(packet.content));
        }
        //////////////////////////////////////////////////////

        //////////////// INDEX EXTRACTED /////////////////////
        while (!data_q.empty()) {
            tmp_content = std::move(data_q.front());
            data_q.pop_front();
            tmp_content = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(tmp_content)));
            ba::ssegment_index map(ba::word, tmp_content.begin(), tmp_content.end());
            map.rule(ba::word_any);
            for (auto it = map.begin(), e = map.end(); it != e; ++it)
                map_of_words[*it] += 1;
            tmp_content.clear();
        }
#ifdef DEBUG_INFO
        std::cout << "." << std::flush;
#endif
        //////////////////////////////////////////////////////
    }

    ////////////////////// CLOSE NEXT PIPE IF NEEDED ///////////////////////
    file_q->emplace_back(file_packet{}); // FORWARD POISON PILL
    map_q->emplace_front(std::move(map_of_words));
    map_q->unpublish(); // IF LAST SEND A POISON PILL!!!
    ////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_INFO
    std::cout << "\nEnd Count!" << std::endl;
#endif
}

static void merge_maps_thread(tqueue_radio<std::map<std::string, size_t>> *queue) {
    queue->subscribe();
    std::vector<std::map<std::string, size_t>> merge_group;
    while (!(merge_group = queue->pop_front_n(2))[0].empty() && !merge_group[1].empty()) {
#ifdef DEBUG_INFO
        std::cout << "+" << std::flush;
#endif
        for (auto &element : merge_group[1])
            merge_group[0][element.first] += merge_group[1][element.first];

        queue->emplace_front_force(std::move(merge_group[0]));
    }

    if (merge_group[1].empty())
        queue->emplace_front(std::move((merge_group[0])));
    else
        queue->emplace_front(std::move(merge_group[1]));

    queue->emplace_back(std::map<std::string, size_t>{}); // PILL
    queue->unsubscribe();
#ifdef DEBUG_INFO
    std::cout << "M" << std::flush;
#endif
}

std::map<std::string, size_t> parallel_count(const std::vector<std::string> &file_list,
                                             const uint8_t number_of_threads) {
    std::vector<std::thread> vector_of_threads{};

    t_queue<file_packet> loader_queue{MAX_LOAD_QUEUE_SIZE};
    tqueue_radio<std::string> data_queue{static_cast<size_t> (number_of_threads) * MAX_DATA_QUEUE_SIZE_PER_THREAD};
    tqueue_radio<std::map<std::string, size_t>> map_queue{};

#ifdef VISUAL_PIPELINE
    volatile bool cond = true;
    std::thread visual_t{visual_pipeline, &cond, loader_queue, &data_queue, &map_queue};
#endif

    for (uint8_t i = 0; i < number_of_threads; i++)
        vector_of_threads.emplace_back(count_thread, &loader_queue, &map_queue);

    for (uint8_t i = 0; i < MERGE_THREADS; i++)
        vector_of_threads.emplace_back(merge_maps_thread, &map_queue);

    read_files_thread(file_list, &loader_queue);

    for (auto &t: vector_of_threads)
        t.join();

#ifdef VISUAL_PIPELINE
    cond = false;
    visual_t.join();
#endif

    return map_queue.pop_front();
}
