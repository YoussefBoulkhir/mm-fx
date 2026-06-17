#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

// Queue thread-safe (Producer / Consumer)
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
    bool stopped = false;

public:

    void push(const T& value) {
        {
            std::lock_guard lock(m);
            q.push(value);
        }
        cv.notify_one();
    }

    bool pop(T& out) {
        std::unique_lock lock(m);

        cv.wait(lock, [&] {
            return stopped || !q.empty();
        });

        if (stopped && q.empty())
            return false;

        out = q.front();
        q.pop();
        return true;
    }

    void stop() {
        {
            std::lock_guard lock(m);
            stopped = true;
        }
        cv.notify_all();
    }
};