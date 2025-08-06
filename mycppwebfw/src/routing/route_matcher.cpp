#include "mycppwebfw/routing/route_matcher.h"
#include <regex>
#include <sstream>

namespace mycppwebfw
{
namespace routing
{

std::shared_ptr<TrieNode>
RouteMatcher::match(const std::shared_ptr<TrieNode>& root,
                    const std::string& path,
                    std::unordered_map<std::string, std::string>& params)
{
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

    std::vector<std::shared_ptr<TrieNode>> matches;
    std::function<void(std::shared_ptr<TrieNode>, size_t,
                       std::unordered_map<std::string, std::string>)>
        find_matches =
            [&](std::shared_ptr<TrieNode> current, size_t segment_index,
                std::unordered_map<std::string, std::string> current_params)
    {
        if (current->is_static_files)
        {
            matches.push_back(current);
            return;
        }

        if (segment_index == segments.size())
        {
            if (current->handler)
            {
                matches.push_back(current);
            }
            for (auto const& [key, val] : current->children)
            {
                if (val->is_optional)
                {
                    if (val->handler)
                    {
                        matches.push_back(val);
                    }
                }
            }
            return;
        }

        const auto& seg = segments[segment_index];
        if (current->children.find(seg) != current->children.end())
        {
            find_matches(current->children[seg], segment_index + 1,
                         current_params);
        }

        for (auto const& [key, val] : current->children)
        {
            if (val->type == NodeType::PARAMETER)
            {
                auto next_params = current_params;
                next_params[val->part] = seg;
                find_matches(val, segment_index + 1, next_params);
            }
            else if (val->type == NodeType::WILDCARD)
            {
                std::string remaining_path;
                for (size_t i = segment_index; i < segments.size(); ++i)
                {
                    remaining_path += segments[i];
                    if (i < segments.size() - 1)
                    {
                        remaining_path += "/";
                    }
                }
                auto next_params = current_params;
                next_params[val->part] = remaining_path;
                find_matches(val, segments.size(), next_params);
            }
            else if (val->type == NodeType::REGEX)
            {
                try
                {
                    std::regex pattern(val->regex_pattern);
                    if (std::regex_match(seg, pattern))
                    {
                        auto next_params = current_params;
                        next_params[val->part] = seg;
                        find_matches(val, segment_index + 1, next_params);
                    }
                }
                catch (const std::regex_error& e)
                {
                    // Handle regex compilation error
                }
            }
        }
    };

    find_matches(root, 0, {});

    if (matches.empty())
    {
        if (root->is_catch_all)
        {
            return root;
        }
        return nullptr;
    }

    std::sort(matches.begin(), matches.end(), [](const auto& a, const auto& b)
              { return a->priority > b->priority; });

    auto best_match = matches[0];
    params.clear();

    // Recalculate params for the best match
    std::function<bool(std::shared_ptr<TrieNode>, size_t,
                       std::unordered_map<std::string, std::string>&)>
        recalculate_params =
            [&](std::shared_ptr<TrieNode> current, size_t segment_index,
                std::unordered_map<std::string, std::string>& out_params)
        -> bool
    {
        if (segment_index == segments.size())
        {
            return current == best_match;
        }

        const auto& seg = segments[segment_index];
        if (current->children.count(seg))
        {
            if (recalculate_params(current->children[seg], segment_index + 1,
                                   out_params))
            {
                return true;
            }
        }

        for (auto const& [key, val] : current->children)
        {
            if (val->type == NodeType::PARAMETER)
            {
                if (recalculate_params(val, segment_index + 1, out_params))
                {
                    out_params[val->part] = seg;
                    return true;
                }
            }
            else if (val->type == NodeType::WILDCARD)
            {
                if (val == best_match)
                {
                    std::string remaining_path;
                    for (size_t i = segment_index; i < segments.size(); ++i)
                    {
                        remaining_path += segments[i];
                        if (i < segments.size() - 1)
                        {
                            remaining_path += "/";
                        }
                    }
                    out_params[val->part] = remaining_path;
                    return true;
                }
            }
            else if (val->type == NodeType::REGEX)
            {
                try
                {
                    std::regex pattern(val->regex_pattern);
                    if (std::regex_match(seg, pattern))
                    {
                        if (recalculate_params(val, segment_index + 1,
                                               out_params))
                        {
                            out_params[val->part] = seg;
                            return true;
                        }
                    }
                }
                catch (const std::regex_error& e)
                {
                    // Handle regex compilation error
                }
            }
        }
        return false;
    };

    recalculate_params(root, 0, params);

    if (best_match->is_optional && !best_match->default_value.empty())
    {
        if (params.find(best_match->part) == params.end())
        {
            params[best_match->part] = best_match->default_value;
        }
    }

    return best_match;
}

}  // namespace routing
}  // namespace mycppwebfw
