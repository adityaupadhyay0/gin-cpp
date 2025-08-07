#pragma once

#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/context/context.h"
#include <functional>
#include <memory>

namespace mycppwebfw {
namespace middleware {

using Next = std::function<void()>;

class IMiddleware {
public:
    virtual ~IMiddleware() = default;
    virtual void operator()(Context& ctx, Next next) = 0;
};

} // namespace middleware
} // namespace mycppwebfw
