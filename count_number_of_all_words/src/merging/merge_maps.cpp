//
// Created by myralllka on 4/4/20.
//

#include "../../includes/merging/merge_maps.h"
#include <map>
#include <string>
#include <iostream>

inline void print_map(std::map<std::string, int> map) {
    for (auto &pair:map) {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
}

void merge_maps_queue(t_queue<std::map<std::string, int>> &queue, uint8_t num_of_threads) {
//    while (!queue.peek_front().empty()) {
//        std::cout << "oh shit" << std::endl;
//    while (queue.get_size() > 1) {
    for (; num_of_threads > 1; num_of_threads--) {
        auto map1 = queue.pop_front();
        auto map2 = queue.pop_front();
        for (auto &element:map1) {
            map2[element.first] += map1[element.first];
        }
        queue.emplace_back(std::move(map2));
    }
//    queue.emplace_back(std::map<std::string, int>{});
}
