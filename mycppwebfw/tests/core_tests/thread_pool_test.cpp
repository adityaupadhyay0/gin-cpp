// Test: thread_pool_test
#include "mycppwebfw/core/thread_pool.h"
#include <gtest/gtest.h>
#include <atomic>

TEST(ThreadPoolTest, ConstructionAndDestruction) {
    mycppwebfw::core::ThreadPoolConfig config;
    mycppwebfw::core::ThreadPool pool(config);
    SUCCEED();
}

TEST(ThreadPoolTest, EnqueueAndExecuteTask) {
    mycppwebfw::core::ThreadPoolConfig config;
    config.min_threads = 1;
    mycppwebfw::core::ThreadPool pool(config);
    std::atomic<bool> task_executed = false;

    pool.enqueue([&task_executed] {
        task_executed = true;
    });

    // Busy wait for the task to be executed.
    // In a real application, you would use a future or a condition variable.
    for (int i = 0; i < 100 && !task_executed; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ASSERT_TRUE(task_executed);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
