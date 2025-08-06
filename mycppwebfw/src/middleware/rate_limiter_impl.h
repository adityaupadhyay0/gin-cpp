#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include "middleware/rate_limiter.h"
#include "utils/token_bucket.h"

namespace mycppwebfw
{
namespace middleware
{

struct RateLimiter::Impl
{
    Impl(double tokens_per_second, size_t burst_size)
        : tokens_per_second_(tokens_per_second), burst_size_(burst_size)
    {
    }

    utils::TokenBucket& get_bucket(const std::string& ip);

    double tokens_per_second_;
    size_t burst_size_;
    std::unordered_map<std::string, utils::TokenBucket> clients_;
    std::mutex mutex_;
};

}  // namespace middleware
}  // namespace mycppwebfw
