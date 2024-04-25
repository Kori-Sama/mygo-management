#include "thread_pool.h"


ThreadPool::ThreadPool(unsigned int num)
    : _stop(false) {
        {
            if (num <= 1)
                _thread_num = 2;
            else
                _thread_num = num;
        }
        start();
}

ThreadPool::~ThreadPool() {
    stop();
}

ThreadPool& ThreadPool::instance() {
    static ThreadPool instance;
    return instance;
}

void ThreadPool::add_task(Task&& task) {
    {
        std::unique_lock<std::mutex> cv_mt(_cv_mt);
        _tasks.push(std::move(task));
    }
    _cv_lock.notify_one();
}

int ThreadPool::idle_thread_num() {
    return _thread_num;
}

void ThreadPool::start() {
    for (int i = 0; i < _thread_num; ++i) {
        _pool.emplace_back([this]() {
            while (!this->_stop.load()) {
                Task task;
                {
                    std::unique_lock<std::mutex> cv_mt(_cv_mt);
                    this->_cv_lock.wait(cv_mt, [this] {
                        return this->_stop.load() || !this->_tasks.empty();
                        });
                    if (this->_tasks.empty())
                        return;

                    task = std::move(this->_tasks.front());
                    this->_tasks.pop();
                }
                this->_thread_num--;
                task();
                this->_thread_num++;
            }
            });
    }
}

void ThreadPool::stop() {
    _stop.store(true);
    _cv_lock.notify_all();
    for (auto& td : _pool) {
        if (td.joinable()) {
            std::cout << "join thread " << td.get_id() << std::endl;
            td.join();
        }
    }
}