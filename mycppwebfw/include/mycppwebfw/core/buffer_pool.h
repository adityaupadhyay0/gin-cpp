

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
