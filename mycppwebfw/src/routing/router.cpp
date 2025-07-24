#include "mycppwebfw/routing/router.h"
#include <sstream>
#include "mycppwebfw/routing/route_matcher.h"
#include "mycppwebfw/src/utils/file_server.h"
#include "mycppwebfw/utils/logger.h"

namespace mycppwebfw
{
namespace routing
{

Router::Router()
{
}

Router::~Router()
{
}

RouteGroup::RouteGroup(const std::string& prefix, Router* router,
                       const std::vector<HttpHandler>& middlewares)
    : prefix(prefix), router(router), middlewares(middlewares)
{
}

void RouteGroup::add_route(const std::string& method, const std::string& path,
                           HttpHandler handler,
                           const std::vector<HttpHandler>& route_middlewares,
                           const std::string& name, int priority)
{
    std::vector<HttpHandler> all_middlewares = middlewares;
    all_middlewares.insert(all_middlewares.end(), route_middlewares.begin(),
                           route_middlewares.end());
    router->add_route(method, prefix + path, handler, all_middlewares, name,
                      priority);
}

void RouteGroup::group(const std::string& new_prefix,
                       const std::vector<HttpHandler>& group_middlewares,
                       const std::function<void(RouteGroup&)>& group_routes)
{
    std::vector<HttpHandler> all_middlewares = middlewares;
    all_middlewares.insert(all_middlewares.end(), group_middlewares.begin(),
                           group_middlewares.end());
    RouteGroup new_group(prefix + new_prefix, router, all_middlewares);
    group_routes(new_group);
}

void Router::add_static_route(const std::string& path,
                              const std::string& base_dir)
{
    if (trees.find("GET") == trees.end())
    {
        trees["GET"] = std::make_shared<TrieNode>("", NodeType::STATIC);
    }
    std::shared_ptr<TrieNode> root = trees["GET"];
    std::stringstream ss(path);
    std::string segment;
    std::vector<std::string> segments;
    while (std::getline(ss, segment, '/'))
    {
        if (!segment.empty())
        {
            segments.push_back(segment);
        }
    }

    std::shared_ptr<TrieNode> current = root;
    for (const auto& seg : segments)
    {
        if (current->children.find(seg) == current->children.end())
        {
            current->children[seg] =
                std::make_shared<TrieNode>(seg, NodeType::STATIC);
        }
        current = current->children[seg];
    }
    current->is_static_files = true;
    current->static_files_base_dir = base_dir;
}

void Router::add_route(const std::string& method, const std::string& path,
                       HttpHandler handler,
                       const std::vector<HttpHandler>& middlewares,
                       const std::string& name, int priority)
{
    if (!name.empty())
    {
        if (named_routes.count(name))
        {
            throw std::runtime_error("Route name '" + name +
                                     "' is already defined.");
        }
        named_routes[name] = path;
        LOG_DEBUG("Registered route '" + path + "' with name '" + name + "'.");
    }

    if (trees.find(method) == trees.end())
    {
        trees[method] = std::make_shared<TrieNode>("", NodeType::STATIC);
    }
    std::shared_ptr<TrieNode> root = trees[method];
    std::stringstream ss(path);
    std::string segment;
    std::vector<std::string> segments;
    while (std::getline(ss, segment, '/'))
    {
        if (!segment.empty())
        {
            segments.push_back(segment);
        }
    }

    if (path == "/")
    {
        root->is_catch_all = true;
        root->handler = handler;
        root->middlewares = middlewares;
        return;
    }

    std::shared_ptr<TrieNode> current = root;
    for (const auto& seg : segments)
    {
        NodeType type = NodeType::STATIC;
        std::string part = seg;
        bool is_optional = false;
        std::string default_value;

        if (seg.front() == ':')
        {
            type = NodeType::PARAMETER;
            part = seg.substr(1);
            if (part.back() == '?')
            {
                is_optional = true;
                part.pop_back();
            }
            size_t equals_pos = part.find('=');
            if (equals_pos != std::string::npos)
            {
                default_value = part.substr(equals_pos + 1);
                part = part.substr(0, equals_pos);
                is_optional = true;
            }
        }
        else if (seg.front() == '*')
        {
            type = NodeType::WILDCARD;
            part = seg.substr(1);
            current->is_wildcard = true;
        }
        else if (seg.front() == '<' && seg.back() == '>')
        {
            type = NodeType::REGEX;
            part = seg.substr(1, seg.length() - 2);
            current->regex_pattern = part;
        }

        if (current->children.find(part) == current->children.end())
        {
            current->children[part] = std::make_shared<TrieNode>(part, type);
        }
        current = current->children[part];
        if (type == NodeType::PARAMETER)
        {
            current->is_optional = is_optional;
            current->default_value = default_value;
        }
    }
    if (current->handler != nullptr)
    {
        LOG_WARNING("Route conflict: " + path +
                    " conflicts with an existing route.");
    }
    current->handler = handler;
    current->middlewares = middlewares;
    if (priority != 0)
    {
        current->priority = priority;
    }
    else
    {
        if (current->type == NodeType::STATIC)
        {
            current->priority = 3;
        }
        else if (current->type == NodeType::PARAMETER)
        {
            current->priority = 2;
        }
        else if (current->type == NodeType::WILDCARD)
        {
            current->priority = 1;
        }
    }
}

void Router::group(const std::string& prefix,
                   const std::function<void(RouteGroup&)>& group_routes)
{
    LOG_DEBUG("Creating route group with prefix '" + prefix + "'.");
    RouteGroup route_group(prefix, this);
    group_routes(route_group);
}

void Router::group(const std::string& prefix,
                   const std::vector<HttpHandler>& middlewares,
                   const std::function<void(RouteGroup&)>& group_routes)
{
    LOG_DEBUG("Creating route group with prefix '" + prefix + "' and " +
              std::to_string(middlewares.size()) + " middlewares.");
    RouteGroup route_group(prefix, this, middlewares);
    group_routes(route_group);
}

std::string
Router::url_for(const std::string& name,
                const std::unordered_map<std::string, std::string>& params)
{
    if (named_routes.find(name) == named_routes.end())
    {
        LOG_WARNING("Route with name '" + name + "' not found.");
        return "";
    }
    std::string path = named_routes[name];
    LOG_DEBUG("Generating URL for named route '" + name + "' with path '" +
              path + "'.");

    for (const auto& [key, value] : params)
    {
        std::string placeholder = ":" + key;
        size_t pos = path.find(placeholder);
        if (pos != std::string::npos)
        {
            path.replace(pos, placeholder.length(), value);
        }
        else
        {
            LOG_WARNING("Parameter '" + key +
                        "' not found in route path for '" + name + "'.");
        }
    }
    return path;
}

Router::MatchResult
Router::match_route(http::Request& request,
                    std::unordered_map<std::string, std::string>& params)
{
    std::string original_method = request.get_method();
    std::string effective_method = original_method;
    auto override_header = request.get_header("X-HTTP-Method-Override");
    if (!override_header.empty())
    {
        effective_method = override_header;
        LOG_DEBUG("Method overridden by X-HTTP-Method-Override header to: " +
                  effective_method);
    }
    else
    {
        auto query_params = request.get_query_params();
        if (query_params.count("_method"))
        {
            effective_method = query_params.at("_method");
            LOG_DEBUG("Method overridden by _method query parameter to: " +
                      effective_method);
        }
    }

    LOG_INFO("Matching route: " + effective_method + " " + request.get_path());

    MatchResult result;
    if (trees.find(effective_method) != trees.end())
    {
        auto node = RouteMatcher::match(trees[effective_method],
                                        request.get_path(), params);
        if (node)
        {
            if (node->is_static_files)
            {
                utils::FileServer file_server(node->static_files_base_dir);
                result.handler = [&](http::Request& req, http::Response& res)
                { file_server.handle_request(req, res); };
                return result;
            }
            LOG_INFO("Route matched");
            result.handler = node->handler;
            result.middlewares = node->middlewares;
            return result;
        }
    }

    LOG_WARNING("Route not found for method: " + effective_method);

    for (const auto& [tree_method, tree] : trees)
    {
        if (tree_method != effective_method)
        {
            std::unordered_map<std::string, std::string> temp_params;
            if (RouteMatcher::match(tree, request.get_path(), temp_params))
            {
                result.allowed_methods.push_back(tree_method);
            }
        }
    }

    if (!result.allowed_methods.empty())
    {
        LOG_WARNING("Path found for other methods, returning 405");
        result.handler = [](http::Request&, http::Response& res)
        { res.status = http::Response::StatusCode::bad_request; };
    }
    else
    {
        LOG_WARNING("No route found for path");
    }

    return result;
}

}  // namespace routing
}  // namespace mycppwebfw
