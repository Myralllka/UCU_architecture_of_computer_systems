//
// Created by solia on 4/5/20.
//

#include "../includes/parallel_program.h"
#include "../includes/linear_extractor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <boost/locale.hpp>
#include <thread>


void count_words(std::vector<std::string> &data, const int start_position, const int end_position,
        std::map<std::string, int> &map_of_words) {
    std::string word;

    typedef std::vector<std::string>::iterator vec_iter;
    for (vec_iter cur = data.begin() + start_position; cur != data.begin() + end_position; cur++) {
        std::string element = *cur;
        element = boost::locale::to_lower(boost::locale::fold_case(boost::locale::normalize(element)));
        element.erase(std::remove_if(element.begin(), element.end(),
                                     [](const unsigned &c) { return !isspace(c) && !isalpha(c); }), element.end());
        for (auto &chr:element) {
            if (isalpha(chr))
                word += tolower(chr);
            else if (isspace(chr)) {
                auto itr = map_of_words.find(word);
                if (itr != map_of_words.end()) {
                    map_of_words[word] += 1;
                } else {
                    map_of_words[word] = 1;
                }
                word.clear();
            }
        }
    }
    for (auto &pair:map_of_words) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void parallel_count(const std::string &input_filename, const std::string &output_filename_a,
                    const std::string &output_filename_n, const int no_threads){
    // read entire binary archive into the buffer
    std::ifstream raw_file(input_filename, std::ios::binary);
    std::vector<std::string> data;

    // read buffer
    auto buffer = [&raw_file] {
        std::ostringstream ss{};
        ss << raw_file.rdbuf();
        return ss.str();
    }();
    extract_to_memory(buffer, &data);

    // divide data
    int data_len = data.size();
    double tmp = data_len / no_threads;
    int data_portion_len = static_cast<int>(tmp + 0.5 - (tmp < 0));

    // parallel counting
    std::vector<std::map<std::string, int>> vector_of_maps;
    std::vector<std::thread> vector_of_threads;
    for (size_t i = 0;  i < no_threads; i++) {
        std::map<std::string, int> map;
        vector_of_maps.push_back(map);

        vector_of_threads.push_back(std::thread(count_words, std::ref(data), static_cast<int>(i) * data_portion_len,
                                                            std::min(static_cast<int>(i + 1) * data_portion_len, data_len),
                                                            std::ref(vector_of_maps[i])));
    }
    for (auto& t: vector_of_threads) {
        t.join();
    }
    vector_of_threads.clear();
}
