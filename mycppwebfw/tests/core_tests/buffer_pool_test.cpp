#include <gtest/gtest.h>
#include "mycppwebfw/core/buffer_pool.h"

TEST(BufferPoolTest, AcquireRelease) {
    BufferPool<> pool(1024, 4);
    auto buf1 = pool.acquire();
    ASSERT_TRUE(buf1 != nullptr);
    pool.release(buf1);
    ASSERT_EQ(pool.getPoolUsage(), 4);
}

TEST(BufferPoolTest, MemoryLimit) {
    BufferPool<> pool(1024, 2, 2048); // 2KB max
    auto buf1 = pool.acquire();
    auto buf2 = pool.acquire();
    auto buf3 = pool.acquire(); // Should be nullptr (limit)
    ASSERT_TRUE(buf1 != nullptr);
    ASSERT_TRUE(buf2 != nullptr);
    ASSERT_TRUE(buf3 == nullptr);
}

#include <thread>
#include <vector>
#include <atomic>

TEST(BufferPoolTest, ConcurrentAcquireRelease) {
    BufferPool<> pool(256, 8);
    std::atomic<int> acquired{0};
    std::vector<std::thread> threads;
    for (int i = 0; i < 16; ++i) {
        threads.emplace_back([&]() {
            auto buf = pool.acquire();
            if (buf) {
                acquired++;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                pool.release(buf);
            }
        });
    }
    for (auto& t : threads) t.join();
    ASSERT_EQ(pool.getPoolUsage(), 8);
    ASSERT_EQ(acquired, 16);
}

TEST(BufferPoolTest, CustomAllocatorSupport) {
    using MyAlloc = std::allocator<char>;
    BufferPool<MyAlloc> pool(128, 2, 0, MyAlloc{});
    auto buf = pool.acquire();
    ASSERT_TRUE(buf != nullptr);
    pool.release(buf);
    ASSERT_EQ(pool.getPoolUsage(), 2);
}

TEST(BufferPoolTest, StressTestManyBuffers) {
    BufferPool<> pool(64, 32, 2048);
    std::vector<std::shared_ptr<std::vector<char>>> bufs;
    for (int i = 0; i < 100; ++i) {
        auto buf = pool.acquire();
        if (buf) bufs.push_back(buf);
    }
    for (auto& buf : bufs) pool.release(buf);
    ASSERT_LE(pool.getCurrentMemoryUsage(), pool.getMaxMemoryLimit());
}

TEST(BufferPoolTest, EdgeCaseZeroPoolSize) {
    BufferPool<> pool(128, 0);
    auto buf = pool.acquire();
    ASSERT_TRUE(buf != nullptr);
    pool.release(buf);
    ASSERT_EQ(pool.getPoolUsage(), 0);
}
