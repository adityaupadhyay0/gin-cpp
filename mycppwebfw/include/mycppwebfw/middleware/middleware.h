#pragma once

#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include <functional>

namespace mycppwebfw
{

using HttpHandler = std::function<void(http::Request&, http::Response&)>;

namespace middleware
{

using Next = std::function<void()>;

struct Middleware
{
    std::function<void(http::Request&, http::Response&, Next)> handler;
    int priority = 0;

    bool operator<(const Middleware& other) const
    {
        return priority < other.priority;
    }
};

} // namespace middleware
} // namespace mycppwebfw
