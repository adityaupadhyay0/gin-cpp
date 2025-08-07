#include "mycppwebfw/middleware/session.h"
#include "mycppwebfw/utils/request_id_generator.h"

namespace mycppwebfw {
namespace middleware {

// A simple function to parse cookies from the Cookie header.
std::unordered_map<std::string, std::string> parse_cookies(const std::string& cookie_header) {
    std::unordered_map<std::string, std::string> cookies;
    std::string::size_type pos = 0;
    while (pos < cookie_header.length()) {
        auto end = cookie_header.find(';', pos);
        if (end == std::string::npos) {
            end = cookie_header.length();
        }
        auto pair = cookie_header.substr(pos, end - pos);
        auto eq = pair.find('=');
        if (eq != std::string::npos) {
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

Session::Session(std::shared_ptr<utils::SessionStore> store) : store_(store) {}

void Session::operator()(Context& ctx, Next next) {
    auto& req = ctx.req();
    auto& res = ctx.res();
    auto cookie_header = req.get_header("Cookie");
    auto cookies = parse_cookies(cookie_header);
    std::string session_id;
    auto it = cookies.find("session_id");
    if (it != cookies.end()) {
        session_id = it->second;
    } else {
        session_id = utils::RequestIdGenerator::generate();
        res.headers.push_back({"Set-Cookie", "session_id=" + session_id + "; HttpOnly; Secure"});
    }

    ctx.set("session_id", session_id);
    next();
}

std::shared_ptr<IMiddleware> create_session_middleware(
    std::shared_ptr<utils::SessionStore> store) {
    return std::make_shared<Session>(store);
}

} // namespace middleware
} // namespace mycppwebfw
