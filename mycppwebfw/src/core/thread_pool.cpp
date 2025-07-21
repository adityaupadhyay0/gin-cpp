#include "mycppwebfw/core/thread_pool.h"

namespace mycppwebfw {
namespace core {

ThreadPool::ThreadPool(const ThreadPoolConfig& config) : config_(config), stop_(false) {
    for (size_t i = 0; i < config_.min_threads; ++i) {
        workers_.emplace_back(&ThreadPool::worker_thread, this);
    }
}

ThreadPool::~ThreadPool() {
    stop_ = true;
    condition_.notify_all();
    for (std::thread &worker : workers_) {
        worker.join();
    }
}

ThreadPool::ThreadPool(ThreadPool&& other) noexcept : config_(other.config_), stop_(other.stop_.load()) {
    other.stop_ = true;
    for (std::thread &worker : other.workers_) {
        if(worker.joinable()) {
            worker.join();
        }
    }
}

ThreadPool& ThreadPool::operator=(ThreadPool&& other) noexcept {
    if (this != &other) {
        config_ = other.config_;
        stop_ = other.stop_.load();
        other.stop_ = true;
        for (std::thread &worker : other.workers_) {
            if(worker.joinable()) {
                worker.join();
            }
        }
    }
    return *this;
}

void ThreadPool::worker_thread() {
    while (!stop_) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) {
                return;
            }
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}

} // namespace core
} // namespace mycppwebfw
