#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <mutex>

template <typename Alloc = std::allocator<char>>
class BufferPool {
public:
    BufferPool(size_t bufferSize, size_t poolSize, size_t maxMemory = 0, const Alloc& alloc = Alloc());
    std::shared_ptr<std::vector<char, Alloc>> acquire();
    void release(std::shared_ptr<std::vector<char, Alloc>> buffer);
    size_t getPoolUsage() const;
    size_t getPoolLimit() const;
    size_t getTotalAllocated() const;
    size_t getCurrentMemoryUsage() const;
    size_t getMaxMemoryLimit() const;
    void setMaxMemoryLimit(size_t maxMemory);
private:
    size_t bufferSize_;
    size_t poolSize_;
    size_t maxMemory_;
    size_t totalAllocated_;
    Alloc allocator_;
    std::deque<std::shared_ptr<std::vector<char, Alloc>>> pool_;
    mutable std::mutex mutex_;
};

template <typename Alloc>
BufferPool<Alloc>::BufferPool(size_t bufferSize, size_t poolSize, size_t maxMemory, const Alloc& alloc)
    : bufferSize_(bufferSize),
      poolSize_(poolSize),
      maxMemory_(maxMemory),
      totalAllocated_(poolSize),
      allocator_(alloc) {
    for (size_t i = 0; i < poolSize_; ++i) {
        pool_.emplace_back(std::make_shared<std::vector<char, Alloc>>(bufferSize_, allocator_));
    }
}

template <typename Alloc>
std::shared_ptr<std::vector<char, Alloc>> BufferPool<Alloc>::acquire() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!pool_.empty()) {
        auto buf = pool_.back();
        pool_.pop_back();
        return buf;
    }
    // Allocate new if pool exhausted, but check memory limit
    if (maxMemory_ > 0 && (totalAllocated_ + 1) * bufferSize_ > maxMemory_) {
        // Memory limit exceeded
        return nullptr;
    }
    ++totalAllocated_;
    return std::make_shared<std::vector<char, Alloc>>(bufferSize_, allocator_);
}

template <typename Alloc>
void BufferPool<Alloc>::release(std::shared_ptr<std::vector<char, Alloc>> buffer) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (pool_.size() < poolSize_) {
        pool_.push_back(buffer);
    } else {
        // Buffer will be freed, reduce totalAllocated
        if (totalAllocated_ > 0) --totalAllocated_;
    }
}

template <typename Alloc>
size_t BufferPool<Alloc>::getPoolUsage() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return pool_.size();
}

template <typename Alloc>
size_t BufferPool<Alloc>::getPoolLimit() const {
    return poolSize_;
}

template <typename Alloc>
size_t BufferPool<Alloc>::getTotalAllocated() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return totalAllocated_;
}

template <typename Alloc>
size_t BufferPool<Alloc>::getCurrentMemoryUsage() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return totalAllocated_ * bufferSize_;
}

template <typename Alloc>
size_t BufferPool<Alloc>::getMaxMemoryLimit() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return maxMemory_;
}

template <typename Alloc>
void BufferPool<Alloc>::setMaxMemoryLimit(size_t maxMemory) {
    std::lock_guard<std::mutex> lock(mutex_);
    maxMemory_ = maxMemory;
}
