#pragma once

#include "mycppwebfw/middleware/base_middleware.h"
#include <memory>

namespace mycppwebfw {
namespace middleware {

class ErrorHandler : public IMiddleware {
public:
    void operator()(Context& ctx, Next next) override;
};

std::shared_ptr<IMiddleware> create_error_handler();

} // namespace middleware
} // namespace mycppwebfw
