//
// Created by solia on 4/5/20.
//

#include "../../includes/counting/parallel_program.h"
#include <vector>
#include <thread>
#include <boost/locale.hpp>
#include "../../includes/archivation/archive_t.h"

#include "tbb/concurrent_queue.h"

namespace ba = boost::locale::boundary;

void merge_maps(
        tbb::concurrent_bounded_queue<std::map<std::string, size_t>> &queue, uint8_t num_of_threads) {
    for (; num_of_threads > 1; num_of_threads--) {
        std::map<std::string, size_t> map1, map2;
        queue.pop(map1);
        queue.pop(map2);
        for (auto &element:map1) {
            map2[element.first] += map1[element.first];
        }
        queue.push(std::move(map2));
    }
}

static void counting(tbb::concurrent_bounded_queue<file_packet> &file_q,
                     tbb::concurrent_bounded_queue<std::map<std::string, size_t>> &map_q) {
    file_packet packet;
    std::deque<std::string> data_q;
    std::map<std::string, size_t> map_of_words{};
    while (true) {
        while (!file_q.try_pop(packet)) {}
        if (packet.empty()) {
            file_q.push(file_packet());
            break;
        }
        if (packet.archived) {
            archive_t tmp_archive{std::move(packet.content)};
            tmp_archive.extract_all(data_q);
        } else {
            data_q.emplace_back(std::move(packet.content));
        }
    }

    for (auto &content:data_q) {
        ba::ssegment_index map(ba::word, content.begin(), content.end());
        map.rule(ba::word_any);
        for(std::string word:map) {
            word = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(word)));
            map_of_words[word] += 1;
        }
        content.clear();
    }
    map_q.push(std::move(map_of_words));
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
    merge_maps(map_queue, number_of_threads);

    for (auto &t: vector_of_threads)
        t.join();
    /////////////////////////////////////////////////////////////////
    map_queue.pop(result);
    dump_map_to_files(result, output_filename_a, output_filename_n);
}

