#pragma once

#include "mycppwebfw/routing/router.h"

namespace mycppwebfw
{
namespace routing
{

class RouteMatcher
{
public:
    static std::shared_ptr<TrieNode>
    match(const std::shared_ptr<TrieNode>& root, const std::string& path,
          std::unordered_map<std::string, std::string>& params);
};

}  // namespace routing
}  // namespace mycppwebfw
