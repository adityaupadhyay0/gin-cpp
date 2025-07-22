#include "mycppwebfw/routing/route_matcher.h"
#include <sstream>

namespace mycppwebfw {
namespace routing {

HttpHandler RouteMatcher::match(const std::shared_ptr<TrieNode>& root, const std::string& path, std::unordered_map<std::string, std::string>& params) {
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
        if (current->children.find(seg) != current->children.end()) {
            current = current->children[seg];
        } else {
            bool found = false;
            for (auto const& [key, val] : current->children) {
                if (val->type == NodeType::PARAMETER) {
                    params[key] = seg;
                    current = val;
                    found = true;
                    break;
                } else if (val->type == NodeType::WILDCARD) {
                    params[key] = path.substr(path.find(seg));
                    return val->handler;
                }
            }
            if (!found) {
                return nullptr;
            }
        }
    }
    return current->handler;
}

} // namespace routing
} // namespace mycppwebfw
