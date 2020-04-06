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

template<typename T>
class t_queue {
private:
    std::deque<T> queue;
    mutable std::mutex mut;
    std::condition_variable cond_variable;

// size consider atomic
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
        cond_variable.notify_one();
    }

    void push_front(T d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.push_front(d);
        }
        cond_variable.notify_one();
    }

    void emplace_back(T &&d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.emplace_back(d);
        }
        cond_variable.notify_one();
    }

    void emplace_front(T d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.emplace_front(d);
        }
        cond_variable.notify_one();
    }

    T pop_front() {
        std::unique_lock<std::mutex> lg(mut);
        cond_variable.wait(lg, [this]() { return queue.size() != 0; });
        T d = queue.front();
        queue.pop_front();
        return d;
    }

    T pop_back() {
        std::unique_lock<std::mutex> lg(mut);
        cond_variable.wait(lg, [this]() { return queue.size() != 0; });
        T d = queue.back();
        queue.pop_back();
        return d;
    }

    size_t get_size() const {
        std::lock_guard<std::mutex> lg(mut);
        return queue.size();
    }
};

#endif //COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_H
