#pragma once

#include "mycppwebfw/middleware/base_middleware.h"
#include <vector>

namespace mycppwebfw {
namespace utils {

class MiddlewareChainBuilder {
public:
    MiddlewareChainBuilder& add(std::shared_ptr<middleware::IMiddleware> middleware);
    void run(Context& ctx);

private:
    std::vector<std::shared_ptr<middleware::IMiddleware>> middlewares_;
};

} // namespace utils
} // namespace mycppwebfw
