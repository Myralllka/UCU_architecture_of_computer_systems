//
// Created by myralllka on 4/5/20.
//
#include <iostream>
#include <fstream>
#include <map>
#include "../includes/print_maps_to_files.h"

void print(const std::map<std::string, int> &map_of_words, const std::string &output_filename_a,
           const std::string &output_filename_n) {
    std::ofstream outfile_alpha;
    std::ofstream outfile_number;
    outfile_alpha.open(output_filename_a);
    outfile_number.open(output_filename_n);

    for (auto &pair:map_of_words) {
        outfile_alpha << pair.first << ": " << pair.second << std::endl;
    }

    auto multimap_of_words = flip_map(map_of_words);

    std::map<int,std::string> :: iterator it;
    for (it=multimap_of_words.begin() ; it!=multimap_of_words.end() ; it++)
        outfile_number << "(" << (*it).first << ", "
             << (*it).second << ")" << std::endl;
//    for (auto &pair:multimap_of_words) {
//        outfile_number << pair.first << ": " << pair.second << std::endl;
//    }
    outfile_alpha.close();
    outfile_number.close();
}