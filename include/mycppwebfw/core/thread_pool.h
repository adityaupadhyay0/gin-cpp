#ifndef MYCPPWEBFW_CORE_THREAD_POOL_H
#define MYCPPWEBFW_CORE_THREAD_POOL_H

#include <vector>
#include <thread>
#include <atomic>
#include <cstddef>
#include <string>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace mycppwebfw {
namespace core {

enum class ThreadPriority {
    Normal,
    High,
    Low
};

struct ThreadPoolConfig {
    size_t min_threads = 4;
    size_t max_threads = 16;
    size_t idle_timeout_ms = 30000;
    size_t queue_size = 10000;
    bool auto_scaling = true;
    ThreadPriority priority = ThreadPriority::Normal;
    std::string thread_name_prefix = "worker";
};

class ThreadPool {
public:
    explicit ThreadPool(const ThreadPoolConfig& config);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) noexcept;
    ThreadPool& operator=(ThreadPool&&) noexcept;

    template<class F>
    void enqueue(F&& f);

private:
    void worker_thread();

    ThreadPoolConfig config_;
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};

template<class F>
void ThreadPool::enqueue(F&& f) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        tasks_.emplace(std::forward<F>(f));
    }
    condition_.notify_one();
}

} // namespace core
} // namespace mycppwebfw

#endif // MYCPPWEBFW_CORE_THREAD_POOL_H
