#include <gtest/gtest.h>
#include "mycppwebfw/middleware/chain.h"

TEST(MiddlewareChainTest, ChainExecution) {
    int counter = 0;
    auto mw1 = [&counter](auto& ctx, auto next) { counter += 1; next(); };
    auto mw2 = [&counter](auto& ctx, auto next) { counter += 2; next(); };
    std::vector<std::function<void(int&, std::function<void()>)>> chain = {mw1, mw2};
    int ctx = 0;
    auto next = [&](){};
    for (auto& mw : chain) mw(ctx, next);
    ASSERT_EQ(counter, 3);
}
