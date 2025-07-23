#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"

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

    TrieNode(const std::string& part, NodeType type) : part(part), type(type)
    {
    }
};

class Router
{
public:
    Router();
    ~Router();

    void add_route(const std::string& method, const std::string& path,
                   HttpHandler handler,
                   const std::vector<HttpHandler>& middlewares = {});
    struct MatchResult
    {
        HttpHandler handler;
        std::vector<HttpHandler> middlewares;
        std::vector<std::string> allowed_methods;
    };

    MatchResult
    match_route(http::Request& request,
                std::unordered_map<std::string, std::string>& params);

private:
    std::unordered_map<std::string, std::shared_ptr<TrieNode>> trees;
};

}  // namespace routing
}  // namespace mycppwebfw
