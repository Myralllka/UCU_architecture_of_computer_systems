//
// Created by myralllka on 3/29/20.
//

#ifndef COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_H
#define COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_H

#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>

template<typename T>
class t_queue {
private:
    std::deque<T> queue;
    mutable std::mutex mut;
    std::condition_variable data_received_notify;
//    std::condition_variable data_published_notify;
//    size_t max_size = 0;

public:
    t_queue() = default;

    ~t_queue() = default;

    t_queue(const t_queue &q) = delete;

    const t_queue &operator=(const t_queue &q) = delete;

    void push_back(T d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.push_back(d);
        }
        data_received_notify.notify_one();
    }

    void push_front(T d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.push_front(d);
        }
        data_received_notify.notify_one();
    }

    void emplace_back(T &&d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.emplace_back(d);
        }
        data_received_notify.notify_one();
    }

    void emplace_front(T d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.emplace_front(d);
        }
        data_received_notify.notify_one();
    }

    std::vector<T> pop_front_n(uint8_t n) {
        std::unique_lock<std::mutex> lg(mut);
        std::vector<T> res{};
        data_received_notify.wait(lg, [this, n]() { return queue.size() >= n; });
        for (uint8_t i = 0; i < n; ++i) {
            res.emplace(res.begin() + i, std::move(queue.front()));
//            res.emplace_back(std::move(queue.front()));
            queue.pop_front();
        }
        return res;
    }

    T pop_front() {
        std::unique_lock<std::mutex> lg(mut);
        data_received_notify.wait(lg, [this]() { return queue.size() != 0; });
        T d = queue.front();
        queue.pop_front();
        return d;
    }

    std::vector<T> pop_back_n(uint8_t n) {
        std::unique_lock<std::mutex> lg(mut);
        std::vector<T> res(n);
        data_received_notify.wait(lg, [this, n]() { return queue.size() >= n; });
        for (uint8_t i = n - 1; i >= 0; --i) {
            res.emplace(i, std::move(queue.back()));
            queue.pop_back();
        }
        return res;
    }

    T pop_back() {
        std::unique_lock<std::mutex> lg(mut);
        data_received_notify.wait(lg, [this]() { return queue.size() != 0; });
        T d = queue.back();
        queue.pop_back();
        return d;
    }

    T peek_front() {
        std::unique_lock<std::mutex> lg(mut);
        data_received_notify.wait(lg, [this]() { return queue.size() != 0; });
        T d = queue.back();
        return d;
    }

    size_t get_size() const {
        std::lock_guard<std::mutex> lg(mut);
        return queue.size();
    }
};


#endif //COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_H
