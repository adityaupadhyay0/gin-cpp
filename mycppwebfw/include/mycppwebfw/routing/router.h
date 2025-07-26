#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/middleware/middleware.h"
#include "mycppwebfw/utils/logger.h"

namespace mycppwebfw
{
namespace routing
{

using HttpHandler = std::function<void(http::Request&, http::Response&)>;

enum class NodeType
{
    STATIC,
    PARAMETER,
    WILDCARD,
    REGEX
};

struct TrieNode
{
    std::string part;
    NodeType type;
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> children;
    HttpHandler handler = nullptr;
    std::vector<mycppwebfw::middleware::Middleware> middlewares;
    bool is_wildcard = false;
    bool is_catch_all = false;
    bool is_static_files = false;
    std::string static_files_base_dir;
    bool is_optional = false;
    std::string default_value;
    std::string regex_pattern;
    int priority = 0;

    TrieNode(const std::string& part, NodeType type) : part(part), type(type)
    {
    }
};

class Router;

class RouteGroup
{
public:
    RouteGroup(
        const std::string& prefix, Router* router,
        const std::vector<mycppwebfw::middleware::Middleware>& middlewares = {});
    void
    add_route(const std::string& method, const std::string& path,
              HttpHandler handler,
              const std::vector<mycppwebfw::middleware::Middleware>& middlewares = {},
              const std::string& name = "", int priority = 0);
    void add_static_route(const std::string& path, const std::string& base_dir);
    void
    group(const std::string& prefix,
          const std::vector<mycppwebfw::middleware::Middleware>& middlewares,
          const std::function<void(RouteGroup&)>& group_routes);

private:
    std::string prefix;
    Router* router;
    std::vector<mycppwebfw::middleware::Middleware> middlewares;
};

namespace devtools
{
class RouteInspector;
}

class Router
{
public:
    Router();
    ~Router();

    void
    add_route(const std::string& method, const std::string& path,
              HttpHandler handler,
              const std::vector<mycppwebfw::middleware::Middleware>& middlewares = {},
              const std::string& name = "", int priority = 0);
    void group(const std::string& prefix,
               const std::function<void(RouteGroup&)>& group_routes);
    void
    group(const std::string& prefix,
          const std::vector<mycppwebfw::middleware::Middleware>& middlewares,
          const std::function<void(RouteGroup&)>& group_routes);
    void add_static_route(const std::string& path, const std::string& base_dir);

    struct MatchResult
    {
        HttpHandler handler;
        std::vector<std::string> allowed_methods;
    };

    MatchResult
    match_route(http::Request& request,
                std::unordered_map<std::string, std::string>& params);

    std::string
    url_for(const std::string& name,
            const std::unordered_map<std::string, std::string>& params = {});

    const std::unordered_map<std::string, std::shared_ptr<TrieNode>>&
    get_trees() const
    {
        return trees;
    }

    void use(mycppwebfw::middleware::Middleware middleware);

private:
    friend class RouteGroup;
    friend class devtools::RouteInspector;
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> trees;
    std::unordered_map<std::string, std::string> named_routes;
    std::vector<mycppwebfw::middleware::Middleware> m_global_middlewares;
};

}  // namespace routing
}  // namespace mycppwebfw
