#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <queue>

#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <thread>

class Barrier {
public:
    Barrier(std::size_t num)
        : num_threads(num),
        wait_count(0),
        instance(0),
        mut(),
        cv()
    {
        if (num == 0) {
            throw std::invalid_argument("Barrier thread count cannot be 0");
        }
    }


    Barrier(const Barrier&) = delete;
    Barrier& operator =(const Barrier&) = delete;

    void wait() {
        std::unique_lock<std::mutex> lock(mut); // acquire lock
        std::size_t inst = instance;

        if (++wait_count == num_threads) {
            wait_count = 0;
            instance++;

            cv.notify_all();
        } else {
            cv.wait(lock, [this, &inst]() { return instance != inst; });
        }
    }
private:
    std::size_t num_threads;
    std::size_t wait_count;
    std::size_t instance;
    std::mutex mut;
    std::condition_variable cv;
};


class ThreadPool {
private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;

public:
    void start();
    void queueJob(const std::function<void()>& job);
    void stop();
    bool busy();
};