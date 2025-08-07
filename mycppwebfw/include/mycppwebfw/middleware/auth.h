#pragma once

#include "mycppwebfw/middleware/middleware.h"
#include "mycppwebfw/utils/api_key_registry.h"
#include "mycppwebfw/utils/jwt_parser.h"
#include <memory>

namespace mycppwebfw {
namespace middleware {

Middleware create_auth_middleware(
    std::shared_ptr<utils::ApiKeyRegistry> api_key_registry,
    std::shared_ptr<utils::JwtParser> jwt_parser);

} // namespace middleware
} // namespace mycppwebfw
