#include "mycppwebfw/middleware/session.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/utils/request_id_generator.h"

namespace mycppwebfw
{
namespace middleware
{

// A simple function to parse cookies from the Cookie header.
std::unordered_map<std::string, std::string>
parse_cookies(const std::string& cookie_header)
{
    std::unordered_map<std::string, std::string> cookies;
    std::string::size_type pos = 0;
    while (pos < cookie_header.length())
    {
        auto end = cookie_header.find(';', pos);
        if (end == std::string::npos)
        {
            end = cookie_header.length();
        }
        auto pair = cookie_header.substr(pos, end - pos);
        auto eq = pair.find('=');
        if (eq != std::string::npos)
        {
            auto key = pair.substr(0, eq);
            auto value = pair.substr(eq + 1);
            // Trim leading whitespace
            key.erase(0, key.find_first_not_of(" \t\n\r\f\v"));
            cookies[key] = value;
        }
        pos = end + 1;
    }
    return cookies;
}

Middleware create_session_middleware(std::shared_ptr<utils::SessionStore> store)
{
    Middleware mw;
    mw.handler = [store](http::Request& req, http::Response& res, Next next)
    {
        auto cookie_header = req.get_header("Cookie");
        auto cookies = parse_cookies(cookie_header);
        std::string session_id;
        auto it = cookies.find("session_id");
        if (it != cookies.end())
        {
            session_id = it->second;
        }
        else
        {
            session_id = utils::RequestIdGenerator::generate();
            res.headers.push_back({"Set-Cookie", "session_id=" + session_id +
                                                     "; HttpOnly; Secure"});
        }

        // In a real application, we would load the session data from the store
        // and make it available to the request handlers.
        // For now, we just pass the session_id to the next middleware.
        req.headers.push_back({"X-Session-ID", session_id});
        next();
    };
    return mw;
}

}  // namespace middleware
}  // namespace mycppwebfw
