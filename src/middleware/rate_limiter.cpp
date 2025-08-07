#include "mycppwebfw/middleware/rate_limiter.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/utils/token_bucket.h"

namespace mycppwebfw {
namespace middleware {

struct RateLimiter::Impl {
    Impl(double tokens_per_second, size_t burst_size)
        : tokens_per_second_(tokens_per_second), burst_size_(burst_size) {}

    double tokens_per_second_;
    size_t burst_size_;
    std::unordered_map<std::string, utils::TokenBucket> clients_;
    std::mutex mutex_;
};

RateLimiter::RateLimiter(double tokens_per_second, size_t burst_size)
    : pimpl_(std::make_unique<Impl>(tokens_per_second, burst_size)) {}

RateLimiter::~RateLimiter() = default;

void RateLimiter::operator()(Context& ctx, Next next) {
    auto& req = ctx.req();
    auto& res = ctx.res();
    std::string ip = "127.0.0.1"; // This should be extracted from the request
    std::lock_guard<std::mutex> lock(pimpl_->mutex_);
    if (pimpl_->clients_.find(ip) == pimpl_->clients_.end()) {
        pimpl_->clients_.emplace(ip, utils::TokenBucket(pimpl_->tokens_per_second_, pimpl_->burst_size_));
    }
    if (!pimpl_->clients_.at(ip).consume()) {
        res.status = http::Response::StatusCode::service_unavailable;
        res.content = "Too many requests";
        return;
    }
    next();
}

} // namespace middleware
} // namespace mycppwebfw
