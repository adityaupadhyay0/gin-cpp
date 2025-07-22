#include "mycppwebfw/routing/router.h"
#include "mycppwebfw/routing/route_matcher.h"
#include "mycppwebfw/utils/logger.h"
#include <sstream>
#include <iostream>

namespace mycppwebfw {
namespace routing {

Router::Router() {}

Router::~Router() {}

void Router::add_route(const std::string& method, const std::string& path, HttpHandler handler) {
    LOG_DEBUG("Adding route: " + method + " " + path);
    if (trees.find(method) == trees.end()) {
        trees[method] = std::make_shared<TrieNode>("", NodeType::STATIC);
    }
    std::shared_ptr<TrieNode> root = trees[method];
    std::stringstream ss(path);
    std::string segment;
    std::vector<std::string> segments;
    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            segments.push_back(segment);
        }
    }

    std::shared_ptr<TrieNode> current = root;
    for (const auto& seg : segments) {
        NodeType type = NodeType::STATIC;
        std::string part = seg;
        if (seg.front() == ':') {
            type = NodeType::PARAMETER;
            part = seg.substr(1);
        } else if (seg.front() == '*') {
            type = NodeType::WILDCARD;
            part = seg.substr(1);
            current->is_wildcard = true;
        }

        if (current->children.find(part) == current->children.end()) {
            current->children[part] = std::make_shared<TrieNode>(part, type);
        }
        current = current->children[part];
    }
    current->handler = handler;
}

HttpHandler Router::match_route(const std::string& method, const std::string& path, std::unordered_map<std::string, std::string>& params) {
    LOG_DEBUG("Matching route: " + method + " " + path);
    if (trees.find(method) == trees.end()) {
        LOG_DEBUG("No routes found for method: " + method);
        return nullptr;
    }
    auto handler = RouteMatcher::match(trees[method], path, params);
    if (handler == nullptr) {
        LOG_DEBUG("No matching route found for: " + path);
    }
    return handler;
}

} // namespace routing
} // namespace mycppwebfw
