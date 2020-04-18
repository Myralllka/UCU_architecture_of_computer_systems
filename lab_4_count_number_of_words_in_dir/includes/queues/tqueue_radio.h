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
    uint32_t publishers = 0;
    std::mutex pub_mux;

    uint32_t subscribers = 0;
    std::mutex sub_mux;

public:
    void subscribe() {
        std::lock_guard<std::mutex> lg(sub_mux);
        subscribers += 1;
    }

    bool unsubscribe() {
        std::lock_guard<std::mutex> lg(sub_mux);
        subscribers -= 1;
        return subscribers == 0;
    }

    [[nodiscard]] uint32_t get_sub_num() const {
        std::lock_guard<std::mutex> lg(sub_mux);
        return subscribers;
    }

    void publish() {
        std::lock_guard<std::mutex> lg(pub_mux);
        publishers += 1;
    }

    bool unpublish(bool if_last_send = true, T to_back_data = T{}) {
        std::lock_guard<std::mutex> lg(pub_mux);
        if (publishers == 1 && if_last_send) {
            push_back(to_back_data);
        }
        publishers -= 1;
        return publishers == 0;
    }

    [[nodiscard]] uint32_t get_pub_num() const {
        std::lock_guard<std::mutex> lg(pub_mux);
        return publishers;
    }
};


#endif //COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_RADIO_H
