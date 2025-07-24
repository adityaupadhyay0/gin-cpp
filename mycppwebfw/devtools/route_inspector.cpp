#include "route_inspector.h"
#include <functional>

namespace mycppwebfw
{
namespace devtools
{

RouteInspector::RouteInspector(routing::Router& router) : router_(router)
{
}

void traverse_trie(const std::string& method, const std::string& current_path,
                   const std::shared_ptr<routing::TrieNode>& node,
                   std::vector<RouteInfo>& routes)
{
    if (node->handler)
    {
        routes.push_back({method, current_path, ""});
    }

    for (const auto& child : node->children)
    {
        std::string next_path = current_path;
        if (!next_path.empty() && next_path.back() != '/')
        {
            next_path += '/';
        }
        next_path += child.first;
        traverse_trie(method, next_path, child.second, routes);
    }
}

std::vector<RouteInfo> RouteInspector::list_routes()
{
    std::vector<RouteInfo> routes;
    const auto& trees = router_.get_trees();

    for (const auto& tree : trees)
    {
        traverse_trie(tree.first, "", tree.second, routes);
    }

    return routes;
}

}  // namespace devtools
}  // namespace mycppwebfw
