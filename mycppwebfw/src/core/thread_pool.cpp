#include "mycppwebfw/core/thread_pool.h"

namespace mycppwebfw {
namespace core {

ThreadPool::ThreadPool(const ThreadPoolConfig& config) : config_(config), stop_(false) {
    for (size_t i = 0; i < config_.min_threads; ++i) {
        stop_flags_.emplace_back(std::make_shared<std::atomic<bool>>(false));
        workers_.emplace_back(&ThreadPool::worker_thread, this, stop_flags_.back());
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

void ThreadPool::worker_thread(std::shared_ptr<std::atomic<bool>> stop_flag) {
    while (!(*stop_flag)) {
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
