#include "mycppwebfw/utils/middleware_utils.h"

namespace mycppwebfw {
namespace utils {

MiddlewareChainBuilder& MiddlewareChainBuilder::add(std::shared_ptr<middleware::IMiddleware> middleware) {
    middlewares_.push_back(middleware);
    return *this;
}

void MiddlewareChainBuilder::run(Context& ctx) {
    auto it = middlewares_.begin();
    std::function<void()> next = [&]() {
        if (it != middlewares_.end()) {
            auto middleware = *it;
            it++;
            (*middleware)(ctx, next);
        }
    };
    next();
}

} // namespace utils
} // namespace mycppwebfw
