//
// Created by solia on 4/5/20.
//

#ifndef ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
#define ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H

#include <string>
#include <vector>
#include <map>
#include "../queues/tqueue.h"
#include "../../includes/queues/tqueue_radio.h"
#include "../files/file_packet.h"
#include "../files/file_interface.h"

#include "../../includes/code_control.h"

#ifdef PROGRESS_BAR

#include "visualization.h"
#include <thread>
#include <iostream>
#include <atomic>
#include <chrono>

#endif

std::map<std::string, size_t> parallel_count(const std::vector<std::string> &file_list,
                                             const uint8_t number_of_threads);

#endif //ARCHITECTURE_OF_COMPUTER_SYSTEMS_PARALLEL_PROGRAM_H
