#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <future>
#include <functional>

namespace thread_pool {
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
        /// @brief Get the instance of the ThreadPool
        /// @return The instance of the ThreadPool 
        static ThreadPool& instance();

        /// @brief Initialize the ThreadPool
        /// @param num The maximum number of threads in the pool 
        void init(int num = std::thread::hardware_concurrency());

        /// @brief Add a task to the ThreadPool
        /// @param task The task to be added
        /// @note The task should be a no return and no parameter function 
        void add_task(Task&& task);

        /// @brief Get the number of idle threads in the ThreadPool
        /// @return The number of idle threads
        int idle_thread_num();

        ~ThreadPool();

        /// @brief Submit a task to the ThreadPool
        /// @tparam F The type of the task
        /// @tparam ...Args The type of the arguments
        /// @param f The task to be submitted 
        /// @param ...args The arguments of the task
        /// @return The future of the task 
        template<typename F, typename... Args>
        auto submit(F&& f, Args&&... args) {
            auto task =
                std::make_shared<std::packaged_task<decltype(f(args...))()>>
                (std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            auto res = task->get_future();
            add_task([task]() { (*task)(); });
            return res;
        }

    private:
        ThreadPool() :_is_stop(false) {};

        void start();
        void stop();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
    };
}

#endif // __THREAD_POOL_H