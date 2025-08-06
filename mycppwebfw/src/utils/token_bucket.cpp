#include "token_bucket.h"
#include <algorithm>

namespace mycppwebfw
{
namespace utils
{

TokenBucket::TokenBucket(double capacity, double refill_rate)
    : capacity_(capacity),
      tokens_(capacity),
      refill_rate_(refill_rate),
      last_refill_time_(std::chrono::steady_clock::now())
{
}

bool TokenBucket::consume(int tokens)
{
    std::lock_guard<std::mutex> lock(mutex_);
    refill();
    if (tokens_ >= tokens)
    {
        tokens_ -= tokens;
        return true;
    }
    return false;
}

void TokenBucket::refill()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - last_refill_time_;
    double tokens_to_add = elapsed.count() * refill_rate_;
    tokens_ = std::min(capacity_, tokens_ + tokens_to_add);
    last_refill_time_ = now;
}

}  // namespace utils
}  // namespace mycppwebfw
