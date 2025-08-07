#pragma once

#include "mycppwebfw/middleware/base_middleware.h"
#include "mycppwebfw/utils/api_key_registry.h"
#include "mycppwebfw/utils/jwt_parser.h"
#include <memory>

namespace mycppwebfw {
namespace middleware {

class Auth : public IMiddleware {
public:
    Auth(std::shared_ptr<utils::ApiKeyRegistry> api_key_registry,
         std::shared_ptr<utils::JwtParser> jwt_parser);

    void operator()(Context& ctx, Next next) override;

private:
    std::shared_ptr<utils::ApiKeyRegistry> api_key_registry_;
    std::shared_ptr<utils::JwtParser> jwt_parser_;
};

std::shared_ptr<IMiddleware> create_auth_middleware(
    std::shared_ptr<utils::ApiKeyRegistry> api_key_registry,
    std::shared_ptr<utils::JwtParser> jwt_parser);

} // namespace middleware
} // namespace mycppwebfw
