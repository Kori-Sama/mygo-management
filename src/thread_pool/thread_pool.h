#pragma once
#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
    using Task = std::function<void()>;
private:
    std::mutex               _cv_mt;
    std::condition_variable  _cv_lock;
    std::atomic_bool         _is_stop;
    std::atomic_int          _thread_num;
    std::queue<Task>         _tasks;
    std::vector<std::thread> _pool;
public:
    static ThreadPool& instance();

    void init(int num = std::thread::hardware_concurrency());

    void add_task(Task&& task);

    int idle_thread_num();

    ~ThreadPool();
private:
    ThreadPool() :_is_stop(false) {};

    void start();
    void stop();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
};