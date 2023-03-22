#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <queue>

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