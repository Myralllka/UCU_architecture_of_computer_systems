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

#define MAP_PACKET_SIZE 10000
#define MAX_DATA_QUEUE_SIZE_PER_THREAD 5
#define MAX_MAP_QUEUE_SIZE 10

static void unarchive_thread(t_queue<file_packet> *file_q, tqueue_radio<std::string> *data_q) {
    data_q->publish();
    file_packet packet;

    while (!(packet = file_q->pop_front()).empty()) {
#ifdef DEBUG_INFO
        std::cout << "*" << std::flush;
#endif
        if (packet.archived) {
            archive_t tmp_archive{std::move(packet.content)};
            tmp_archive.extract_all(data_q, file_q);
        } else {
            data_q->emplace_back(std::move(packet.content));
        }
    }

    file_q->emplace_back(file_packet{}); // forward poison pill
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
        /////////////////////// NORMALIZE CONTENT /////////////////////////
        content = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(content)));
        content.erase(std::remove_if(content.begin(), content.end(),
                                     [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), content.end());
        ///////////////////////////////////////////////////////////////////

        fast_count_words(content, &result_map);

#ifdef ENABLE_MAX_COUNTING_BUFFER
        ///////////////////// PUBLISH FULL MAP ////////////////////////////
        if (result_map.size() > MAP_PACKET_SIZE) {
            map_queue->emplace_back(std::move(result_map));
            result_map = std::map<std::string, size_t>{};
        }
        ///////////////////////////////////////////////////////////////////
#endif
    }

    ///////////////////////// FORWARD POISON PILL //////////////////////////
    data_q->emplace_back("");
    data_q->unsubscribe();
    ////////////////////////////////////////////////////////////////////////

    ////////////////////// CLOSE NEXT PIPE IF NEEDED ///////////////////////
    map_queue->emplace_front(
            std::move(result_map));
    map_queue->unpublish(); // IF LAST SEND A POISON PILL!!!
    ////////////////////////////////////////////////////////////////////////
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
        for (auto &element : merge_group[1])
            merge_group[0][element.first] += merge_group[1][element.first];

        queue->emplace_front(std::move(merge_group[0]));
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

void parallel_count(t_queue<file_packet> *loader_queue,
                    const std::string &output_filename_a, const std::string &output_filename_n,
                    const uint8_t number_of_threads) {
    std::vector<std::thread> vector_of_threads{};

    tqueue_radio<std::string> data_queue{static_cast<size_t> (number_of_threads) * MAX_DATA_QUEUE_SIZE_PER_THREAD};
    tqueue_radio<std::map<std::string, size_t>> map_queue{MAX_MAP_QUEUE_SIZE};

    /////////////////////////// UNARCHIVE ///////////////////////////
    vector_of_threads.emplace_back(unarchive_thread, loader_queue, &data_queue);
    /////////////////////////////////////////////////////////////////

    ///////////////////////////   COUNT   ///////////////////////////
    for (uint8_t i = 0; i < number_of_threads; i++)
        vector_of_threads.emplace_back(count_thread, &data_queue, &map_queue);
    /////////////////////////////////////////////////////////////////

    merge_maps_thread(&map_queue);
    for (auto &t: vector_of_threads)
        t.join();

    dump_map_to_files(map_queue.pop_front(), output_filename_a, output_filename_n);
}
