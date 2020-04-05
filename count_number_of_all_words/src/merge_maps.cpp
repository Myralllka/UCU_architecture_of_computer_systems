//
// Created by myralllka on 4/4/20.
//
#include <map>
#include <string>
#include "../includes/merge_maps.h"

std::map<std::string, int> merge_maps(std::map<std::string, int> &map1, std::map<std::string, int> &map2) {
    map1.insert(map2.begin(), map2.end());
    return map1;
}