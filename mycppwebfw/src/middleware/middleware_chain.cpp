#include "mycppwebfw/middleware/middleware_chain.h"

namespace mycppwebfw
{
namespace middleware
{

MiddlewareChain::MiddlewareChain(std::vector<Middleware> middlewares, HttpHandler final_handler)
    : m_middlewares(std::move(middlewares)), m_final_handler(std::move(final_handler))
{
}

void MiddlewareChain::next(http::Request& req, http::Response& res)
{
  if (m_current_middleware_index < m_middlewares.size())
  {
    auto& middleware = m_middlewares[m_current_middleware_index++];
    middleware.handler(req, res, [this, &req, &res]() { this->next(req, res); });
  }
  else
  {
    m_final_handler(req, res);
  }
}

} // namespace middleware
} // namespace mycppwebfw
