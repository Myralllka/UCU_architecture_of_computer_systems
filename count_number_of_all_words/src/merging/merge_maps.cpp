//
// Created by myralllka on 4/4/20.
//

#include "../../includes/merging/merge_maps.h"


void merge_maps_queue(t_queue<std::map<std::string, int>> &queue) {
    std::map<std::string, int> tmp_map;
    std::map<std::string, int> thread_map{};

    while (!(tmp_map = queue.pop_front()).empty()) {
        thread_map.insert(tmp_map.begin(), tmp_map.end());
    }
    queue.emplace_back(std::move(thread_map));
    queue.emplace_back(std::map<std::string, int>{});
}
