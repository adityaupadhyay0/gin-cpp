#include "mycppwebfw/middleware/auth.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"

namespace mycppwebfw
{
namespace middleware
{

Middleware
create_auth_middleware(std::shared_ptr<utils::ApiKeyRegistry> api_key_registry,
                       std::shared_ptr<utils::JwtParser> jwt_parser)
{
    Middleware mw;
    mw.handler = [api_key_registry, jwt_parser](http::Request& req,
                                                http::Response& res, Next next)
    {
        auto auth_header = req.get_header("Authorization");
        if (auth_header.empty())
        {
            res.status = http::Response::StatusCode::unauthorized;
            res.content = "Unauthorized";
            return;
        }

        std::string user;
        if (auth_header.rfind("Bearer ", 0) == 0)
        {
            std::string token = auth_header.substr(7);
            std::string payload;
            if (jwt_parser->verify(token, payload))
            {
                // In a real application, we would set the user in a context
                // object.
                next();
            }
            else
            {
                res.status = http::Response::StatusCode::unauthorized;
                res.content = "Unauthorized";
            }
        }
        else if (auth_header.rfind("ApiKey ", 0) == 0)
        {
            std::string key = auth_header.substr(7);
            if (api_key_registry->is_valid(key, user))
            {
                // In a real application, we would set the user in a context
                // object.
                next();
            }
            else
            {
                res.status = http::Response::StatusCode::unauthorized;
                res.content = "Unauthorized";
            }
        }
        else
        {
            res.status = http::Response::StatusCode::unauthorized;
            res.content = "Unauthorized";
        }
    };
    return mw;
}

}  // namespace middleware
}  // namespace mycppwebfw
