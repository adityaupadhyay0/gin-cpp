#pragma once

#include "mycppwebfw/middleware/base_middleware.h"
#include <memory>

namespace mycppwebfw {
namespace middleware {

class RateLimiter : public IMiddleware {
public:
    RateLimiter(double tokens_per_second, size_t burst_size);
    ~RateLimiter() override;

    void operator()(Context& ctx, Next next) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace middleware
} // namespace mycppwebfw
