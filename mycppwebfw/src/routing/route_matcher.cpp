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
    size_t segment_index = 0;
    while (segment_index < segments.size()) {
        const auto& seg = segments[segment_index];
        if (current->children.find(seg) != current->children.end()) {
            current = current->children[seg];
            segment_index++;
        } else {
            bool found = false;
            for (auto const& [key, val] : current->children) {
                if (val->type == NodeType::PARAMETER) {
                    params[val->part] = seg;
                    current = val;
                    found = true;
                    segment_index++;
                    break;
                }
            }
            if (found) continue;

            for (auto const& [key, val] : current->children) {
                if (val->type == NodeType::WILDCARD) {
                    std::string remaining_path;
                    for (size_t i = segment_index; i < segments.size(); ++i) {
                        remaining_path += segments[i];
                        if (i < segments.size() - 1) {
                            remaining_path += "/";
                        }
                    }
                    params[val->part] = remaining_path;
                    return val->handler;
                }
            }
            return nullptr;
        }
    }

    if (current && current->handler) {
        return current->handler;
    }

    if (current) {
        for (auto const& [key, val] : current->children) {
            if (val->is_optional) {
                if (!val->default_value.empty()) {
                    params[val->part] = val->default_value;
                }
                if (val->handler) {
                    return val->handler;
                }
            }
        }
    }

    return nullptr;
}

} // namespace routing
} // namespace mycppwebfw
