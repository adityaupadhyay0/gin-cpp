#pragma once

#include <chrono>
#include <mutex>

namespace mycppwebfw
{
namespace utils
{

class TokenBucket
{
public:
    TokenBucket(double capacity, double refill_rate);

    bool consume(int tokens);

private:
    void refill();

    double capacity_;
    double tokens_;
    double refill_rate_;  // tokens per second
    std::chrono::steady_clock::time_point last_refill_time_;
    std::mutex mutex_;
};

}  // namespace utils
}  // namespace mycppwebfw
