//
// Created by solia on 4/5/20.
//

#include "../../includes/counting/parallel_program.h"
#include <vector>
#include <thread>
#include <boost/locale.hpp>
#include "../../includes/archivation/archive_t.h"
#include "../../includes/code_control.h"

#include "tbb/concurrent_queue.h"

namespace ba = boost::locale::boundary;

void merge_maps(
        tbb::concurrent_bounded_queue<std::map<std::string, size_t>> &queue, uint8_t num_of_threads) {
#ifdef DEBUG_INFO
    std::cout << "\nStart Merge!" << std::endl;
#endif
    std::map<std::string, size_t> tmp_map, res_map{};
    for (uint8_t map_id = 0; map_id < num_of_threads; ++map_id) {
        queue.pop(tmp_map);
        for (const auto &element : tmp_map) {
            res_map[element.first] += tmp_map[element.first];
        }
        tmp_map.clear();
    }
    queue.push(std::move(res_map));
}

static void counting(tbb::concurrent_bounded_queue<file_packet> &file_q,
                     tbb::concurrent_bounded_queue<std::map<std::string, size_t>> &map_q) {
    file_packet packet;
    std::deque<std::string> data_q;
    std::map<std::string, size_t> map_of_words{};
    std::string tmp_content;

    while (true) {
        file_q.pop(packet);
        ////////////////// POISON PILL ///////////////////////
        if (packet.empty()) {
            file_q.push(file_packet());
            break;
        }
        //////////////////////////////////////////////////////

        //////////////// LOAD FILE ///////////////////////////
        if (packet.archived) {
            archive_t tmp_archive{std::move(packet.content)};
            tmp_archive.extract_all(data_q);
        } else {
            data_q.emplace_back(std::move(packet.content));
        }
        //////////////////////////////////////////////////////

        //////////////// INDEX EXTRACTED /////////////////////
        while (!data_q.empty()) {
            tmp_content = std::move(data_q.front());
            data_q.pop_front();
            ba::ssegment_index map(ba::word, tmp_content.begin(), tmp_content.end());
            map.rule(ba::word_any);
            for(std::string word:map) {
                word = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(word)));
                map_of_words[word] += 1;
            }
            tmp_content.clear();
#ifdef DEBUG_INFO
            std::cout << "." << std::flush;
#endif
        }
        //////////////////////////////////////////////////////
    }
    map_q.push(std::move(map_of_words));
#ifdef DEBUG_INFO
    std::cout << "\nEnd Count!" << std::endl;
#endif
}

void parallel_count(tbb::concurrent_bounded_queue<file_packet> &loader_queue,
                    const std::string &output_filename_a, const std::string &output_filename_n,
                    const uint8_t number_of_threads) {
    std::vector<std::thread> vector_of_threads{};
    tbb::concurrent_bounded_queue<std::map<std::string, size_t>> map_queue;
    std::map<std::string, size_t> result;

    /////////////////////////// UNARCHIVE & COUNT ///////////////////
    for (uint8_t i = 0; i < number_of_threads; i++)
        vector_of_threads.emplace_back(counting, std::ref(loader_queue), std::ref(map_queue));
    /////////////////////////////////////////////////////////////////

    for (auto &t: vector_of_threads)
        t.join();

#ifdef DEBUG_INFO
    std::cout << "\nMap queue size: " << map_queue.size() << std::endl;
#endif
    merge_maps(map_queue, number_of_threads);
    map_queue.pop(result);
    dump_map_to_files(result, output_filename_a, output_filename_n);
}

