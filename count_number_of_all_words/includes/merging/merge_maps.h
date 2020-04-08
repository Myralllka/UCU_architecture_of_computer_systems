//
// Created by fenix on 4/6/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_MERGE_MAPS_H
#define COUNT_NUMBER_OF_ALL_WORDS_MERGE_MAPS_H

#include <string>
#include <map>
#include "../../includes/queues/tqueue.h"

void merge_maps_queue(t_queue<std::map<std::string, size_t>> &queue, uint8_t num_of_threads);


#endif //COUNT_NUMBER_OF_ALL_WORDS_MERGE_MAPS_H
