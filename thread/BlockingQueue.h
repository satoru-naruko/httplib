#pragma once

#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <thread>
#include <queue>

template <typename T>
class BlockingQueue {
public:
    BlockingQueue() {}
    void Enqueue(const T& item) {
        {
            std::lock_guard<std::mutex> lock(m_mutex); 
            const bool was_empty = m_queue.empty();
            m_queue.push(item);
            if (was_empty) {
                m_empty_wait.notify_one();
            }
        }
    }

    T Dequeue() {
        while (true) {
            std::lock_guard<std::mutex> lock(m_mutex); 
            if (m_queue.empty()) {
                m_empty_wait.wait(lock);
            }
            if (m_queue.empty()) {
                continue;
            }
        
            T result = m_queue.front();
            m_queue.pop();
            return result;
        }
    }

    size_t Size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

private:
  mutable std::mutex m_mutex;
  std::condition_variable m_empty_wait;
  std::queue<T> m_queue;
};

#endif

