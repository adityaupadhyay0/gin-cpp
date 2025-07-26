#pragma once

#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/middleware/middleware.h"
#include <functional>
#include <vector>

namespace mycppwebfw
{
namespace middleware
{

using Next = std::function<void()>;

class MiddlewareChain
{
public:
  MiddlewareChain(std::vector<Middleware> middlewares, HttpHandler final_handler);

  void next(http::Request& req, http::Response& res);

private:
  std::vector<Middleware> m_middlewares;
  HttpHandler m_final_handler;
  size_t m_current_middleware_index = 0;
};

} // namespace middleware
} // namespace mycppwebfw
