#include "mycppwebfw/middleware/auth.h"

namespace mycppwebfw {
namespace middleware {

Auth::Auth(std::shared_ptr<utils::ApiKeyRegistry> api_key_registry,
           std::shared_ptr<utils::JwtParser> jwt_parser)
    : api_key_registry_(api_key_registry), jwt_parser_(jwt_parser) {}

void Auth::operator()(Context& ctx, Next next) {
    auto& req = ctx.req();
    auto& res = ctx.res();
    auto auth_header = req.get_header("Authorization");
    if (auth_header.empty()) {
        res.status = http::Response::StatusCode::unauthorized;
        res.content = "Unauthorized";
        return;
    }

    std::string user;
    if (auth_header.rfind("Bearer ", 0) == 0) {
        std::string token = auth_header.substr(7);
        std::string payload;
        if (jwt_parser_->verify(token, payload)) {
            ctx.set("user", payload);
            next();
        } else {
            res.status = http::Response::StatusCode::unauthorized;
            res.content = "Unauthorized";
        }
    } else if (auth_header.rfind("ApiKey ", 0) == 0) {
        std::string key = auth_header.substr(7);
        if (api_key_registry_->is_valid(key, user)) {
            ctx.set("user", user);
            next();
        } else {
            res.status = http::Response::StatusCode::unauthorized;
            res.content = "Unauthorized";
        }
    } else {
        res.status = http::Response::StatusCode::unauthorized;
        res.content = "Unauthorized";
    }
}

std::shared_ptr<IMiddleware> create_auth_middleware(
    std::shared_ptr<utils::ApiKeyRegistry> api_key_registry,
    std::shared_ptr<utils::JwtParser> jwt_parser) {
    return std::make_shared<Auth>(api_key_registry, jwt_parser);
}

} // namespace middleware
} // namespace mycppwebfw
