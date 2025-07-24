#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
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
    WILDCARD
};

struct TrieNode
{
    std::string part;
    NodeType type;
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> children;
    HttpHandler handler = nullptr;
    std::vector<HttpHandler> middlewares;
    bool is_wildcard = false;
    bool is_optional = false;
    std::string default_value;
    int priority = 0;

    TrieNode(const std::string& part, NodeType type) : part(part), type(type)
    {
    }
};

class Router;

class RouteGroup
{
public:
    RouteGroup(const std::string& prefix, Router* router, const std::vector<HttpHandler>& middlewares = {});
    void add_route(const std::string& method, const std::string& path,
                   HttpHandler handler,
                   const std::vector<HttpHandler>& middlewares = {},
                   const std::string& name = "", int priority = 0);
    void group(const std::string& prefix,
               const std::vector<HttpHandler>& middlewares,
               const std::function<void(RouteGroup&)>& group_routes);

private:
    std::string prefix;
    Router* router;
    std::vector<HttpHandler> middlewares;
};

class Router
{
public:
    Router();
    ~Router();

    void add_route(const std::string& method, const std::string& path,
                   HttpHandler handler,
                   const std::vector<HttpHandler>& middlewares = {},
                   const std::string& name = "", int priority = 0);
    void group(const std::string& prefix,
               const std::function<void(RouteGroup&)>& group_routes);
    void group(const std::string& prefix,
                const std::vector<HttpHandler>& middlewares,
                const std::function<void(RouteGroup&)>& group_routes);

    struct MatchResult
    {
        HttpHandler handler;
        std::vector<HttpHandler> middlewares;
        std::vector<std::string> allowed_methods;
    };

    MatchResult
    match_route(http::Request& request,
                std::unordered_map<std::string, std::string>& params);

    std::string
    url_for(const std::string& name,
            const std::unordered_map<std::string, std::string>& params = {});

private:
    friend class RouteGroup;
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> trees;
    std::unordered_map<std::string, std::string> named_routes;
};

}  // namespace routing
}  // namespace mycppwebfw
