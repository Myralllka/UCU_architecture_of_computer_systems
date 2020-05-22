//
// Created by myralllka on 4/5/20.
//

#include <vector>
#include <thread>
#include <boost/locale.hpp>

#include "../../includes/archivation/archive_t.h"
#include "../../includes/counting/parallel_program.h"

namespace ba = boost::locale::boundary;

void merge_maps(t_queue<std::map<std::string, size_t>> &queue) {
    std::vector<std::map<std::string, size_t>> map_pair;
    while ((!(map_pair = queue.pop_front_n(2))[0].empty() && !map_pair[1].empty())) {
        for (auto &element : map_pair[1]) {
            map_pair[0][element.first] += map_pair[1][element.first];
        }
        queue.emplace_front_force(std::move(map_pair[0]));
    }
    queue.emplace_front(std::move(map_pair[1].empty() ? map_pair[0] : map_pair[1]));
    queue.emplace_back(std::map<std::string, size_t>{}); // PILL
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
    map_q.emplace_front(std::move(map_of_words));
}

