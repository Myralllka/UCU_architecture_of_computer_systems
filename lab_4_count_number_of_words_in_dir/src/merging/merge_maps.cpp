//
// Created by myralllka on 4/4/20.
//

#include "../../includes/merging/merge_maps.h"
#include <map>
#include <string>
#include <vector>

#include "../../includes/debug_control.h"

#ifdef DEBUG_INFO

#include <iostream>

#endif

void merge_maps_thread(t_queue<std::map<std::string, size_t>> *queue) {
    std::vector<std::map<std::string, size_t>> merge_group;
    while (!(merge_group = queue->pop_front_n(2))[0].empty() && !merge_group[2].empty()) {
#ifdef DEBUG_INFO
        std::cout << "+" << std::flush;
#endif
        for (auto &element : merge_group[0]) {
            merge_group[1][element.first] += merge_group[0][element.first];
        }
        queue->emplace_back(std::move(merge_group[1]));
    }
    queue->emplace_back(std::map<std::string, size_t>{});
}
