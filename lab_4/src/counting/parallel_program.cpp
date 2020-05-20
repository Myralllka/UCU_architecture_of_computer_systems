//
// Created by myralllka on 4/5/20.
//

#include <vector>
#include <thread>
#include <boost/locale.hpp>

#include "../../includes/archivation/archive_t.h"
#include "../../includes/counting/parallel_program.h"
#include "../../includes/code_control.h"


namespace ba = boost::locale::boundary;

void merge_maps(
        t_queue<std::map<std::string, size_t>> &queue, uint8_t num_of_threads) {
    std::map<std::string, size_t> tmp_map, res_map{};
    for (uint8_t map_id = 0; map_id < num_of_threads; ++map_id) {
        tmp_map = queue.pop_front();
        for (const auto &element : tmp_map) {
            res_map[element.first] += tmp_map[element.first];
        }
        tmp_map.clear();
    }
    queue.emplace_back(std::move(res_map));
}

void counting(t_queue<file_packet> &file_q,
                     t_queue<std::map<std::string, size_t>> &map_q) {
    file_packet packet;
    std::deque<std::string> data_q;
    std::map<std::string, size_t> map_of_words{};
    std::string tmp_content;

    while (true) {
        packet = file_q.pop_front();
        if (packet.empty()) {
            file_q.emplace_back(file_packet());
            break;
        }
        if (packet.archived) {
            archive_t tmp_archive{std::move(packet.content)};
            tmp_archive.extract_all(data_q);
        } else {
            data_q.emplace_back(std::move(packet.content));
        }
        while (!data_q.empty()) {
            tmp_content = std::move(data_q.front());
            data_q.pop_front();
            tmp_content = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(tmp_content)));
            ba::ssegment_index map(ba::word, tmp_content.begin(), tmp_content.end());
            map.rule(ba::word_letters);
            for (auto it = map.begin(), e = map.end(); it != e; ++it)
                ++map_of_words[*it];
            tmp_content.clear();
        }
    }
    map_q.emplace_back(std::move(map_of_words));
}

