//
// Created by fenix on 4/5/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_RADIO_H
#define COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_RADIO_H

#include "tqueue.h"
#include <cstdint>
#include <set>
#include <atomic>

template<typename T>
class tqueue_radio : public t_queue<T> {
    std::atomic_uint16_t pub_n = 0;
    uint32_t publishers = 0;
    std::mutex pub_mux;

    uint32_t subscribers = 0;
    std::mutex sub_mux;

public:
    void subscribe() {
//        std::lock_guard<std::mutex> lg(sub_mux);
//        subscribers += 1;
    }

    void unsubscribe() {
        std::lock_guard<std::mutex> lg(sub_mux);
        subscribers -= 1;
    }

    void get_sub_num() {
        std::lock_guard<std::mutex> lg(sub_mux);
        return subscribers;
    }

    void publish() {
        std::lock_guard<std::mutex> lg(pub_mux);
        publishers += 1;
    }

    void unpublish() {
        std::lock_guard<std::mutex> lg(pub_mux);
        publishers -= 1;
    }

    void get_pub_num() {
        std::lock_guard<std::mutex> lg(pub_mux);
        return publishers;
    }
};


#endif //COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_RADIO_H
