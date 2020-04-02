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

public:
    t_queue() = default;

    ~t_queue() = default;

    t_queue(const t_queue &q) = delete;

    const t_queue &operator=(const t_queue &q) = delete;

    void push(T d) {
        {
            std::lock_guard<std::mutex> lg(mut);
            queue.push_back(d);
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

    T pop() {
        std::unique_lock<std::mutex> lg(mut);
        if (queue.size() == 0) {
            cond_variable.wait(lg);
        }
        cond_variable.wait(lg, [this]() { return queue.size() != 0; });
        int d = queue.front();
        queue.pop_front();
        return d;
    }

    size_t get_size() const {
        std::lock_guard<std::mutex> lg(mut);
        return queue.size();
    }
};

#endif //COUNT_NUMBER_OF_ALL_WORDS_TQUEUE_H
