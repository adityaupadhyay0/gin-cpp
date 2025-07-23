#include "mycppwebfw/routing/router.h"
#include "mycppwebfw/routing/route_matcher.h"
#include <sstream>

namespace mycppwebfw {
namespace routing {

Router::Router() {}

Router::~Router() {}

void Router::add_route(const std::string& method, const std::string& path, HttpHandler handler) {
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
        bool is_optional = false;
        std::string default_value;

        if (seg.front() == ':') {
            type = NodeType::PARAMETER;
            part = seg.substr(1);
            if (part.back() == '?') {
                is_optional = true;
                part.pop_back();
            }
            size_t equals_pos = part.find('=');
            if (equals_pos != std::string::npos) {
                default_value = part.substr(equals_pos + 1);
                part = part.substr(0, equals_pos);
                is_optional = true;
            }
        } else if (seg.front() == '*') {
            type = NodeType::WILDCARD;
            part = seg.substr(1);
            current->is_wildcard = true;
        }

        if (current->children.find(part) == current->children.end()) {
            current->children[part] = std::make_shared<TrieNode>(part, type);
        }
        current = current->children[part];
        if (type == NodeType::PARAMETER) {
            current->is_optional = is_optional;
            current->default_value = default_value;
        }
    }
    current->handler = handler;
}

HttpHandler Router::match_route(const std::string& method, const std::string& path, std::unordered_map<std::string, std::string>& params) {
    if (trees.find(method) == trees.end()) {
        return nullptr;
    }
    return RouteMatcher::match(trees[method], path, params);
}

} // namespace routing
} // namespace mycppwebfw
