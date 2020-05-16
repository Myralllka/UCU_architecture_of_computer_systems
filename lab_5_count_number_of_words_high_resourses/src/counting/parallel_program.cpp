//
// Created by solia on 4/5/20.
//

#include "../../includes/counting/parallel_program.h"
#include <vector>
#include <thread>
#include <boost/locale.hpp>
#include "../../includes/archivation/archive_t.h"
#include "../../includes/code_control.h"

#define MAP_PACKET_SIZE 10000
#define MAX_DATA_QUEUE_SIZE_PER_THREAD 1
#define MAX_MAP_QUEUE_SIZE 10
#define MERGE_THREADS 1
#define UNARCHIVE_THREADS 1

#include "tbb/concurrent_queue.h"

static void unarchive_thread(tbb::concurrent_queue<file_packet, tbb::cache_aligned_allocator<file_packet>> *file_q,
                             tbb::concurrent_queue<std::string, tbb::cache_aligned_allocator<std::string>> *data_q) {
//    data_q->publish();
    file_packet packet;

    while (!file_q->try_pop(packet)) {
        if (packet.archived) {
            archive_t tmp_archive{std::move(packet.content)};
            tmp_archive.extract_all(data_q);
        } else {
            data_q->push(std::move(packet.content));
        }
    }

    file_q->push(file_packet{}); // forward poison pill
//    data_q->unpublish(); // IF LAST SEND A POISON PILL!!!

}

//static void merge_maps_thread(tqueue_radio<std::map<std::string, size_t>> *queue) {
//    queue->subscribe();
//    std::vector<std::map<std::string, size_t>> merge_group;
//    while (!(merge_group = queue->pop_front_n(2))[0].empty() && !merge_group[1].empty()) {
//        for (auto &element : merge_group[1])
//            merge_group[0][element.first] += merge_group[1][element.first];
//
//        queue->emplace_front_force(std::move(merge_group[0]));
//    }
//
//
//    if (merge_group[1].empty())
//        queue->emplace_front(std::move((merge_group[0])));
//    else
//        queue->emplace_front(std::move(merge_group[1]));
//
//
//    queue->emplace_back(std::map<std::string, size_t>{}); // PILL
//    queue->unsubscribe();
//}
//
void parallel_count(tbb::concurrent_queue<file_packet, tbb::cache_aligned_allocator<file_packet>> *loader_queue,
                    const std::string &output_filename_a, const std::string &output_filename_n,
                    const uint8_t number_of_threads) {
    std::vector<std::thread> vector_of_threads{};
//
//    tqueue_radio<std::string> data_queue{static_cast<size_t> (number_of_threads) * MAX_DATA_QUEUE_SIZE_PER_THREAD};
//    tqueue_radio<std::map<std::string, size_t>> map_queue{MAX_MAP_QUEUE_SIZE};
//
//    /////////////////////////// UNARCHIVE ///////////////////////////
//    for (uint8_t i = 0; i < UNARCHIVE_THREADS; i++)
//        vector_of_threads.emplace_back(unarchive_thread, loader_queue, &data_queue);
//    /////////////////////////////////////////////////////////////////
//
//    ///////////////////////////   COUNT   ///////////////////////////
//    for (uint8_t i = 0; i < number_of_threads; i++)
//        vector_of_threads.emplace_back(count_thread, &data_queue, &map_queue);
//    /////////////////////////////////////////////////////////////////
//
//    for (uint8_t i = 0; i < MERGE_THREADS; i++)
//        vector_of_threads.emplace_back(merge_maps_thread, &map_queue);
//
//    for (auto &t: vector_of_threads)
//        t.join();
//
//
//    dump_map_to_files(map_queue.pop_front(), output_filename_a, output_filename_n);
}
