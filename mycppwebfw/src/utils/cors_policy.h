#pragma once

#include <set>
#include <string>
#include <vector>

namespace mycppwebfw
{
namespace utils
{

struct CorsPolicy
{
    std::set<std::string> allowed_origins;
    std::set<std::string> allowed_methods;
    std::set<std::string> allowed_headers;
    bool allow_credentials = false;
    long max_age = -1;  // in seconds

    bool is_origin_allowed(const std::string& origin) const;
};

}  // namespace utils
}  // namespace mycppwebfw
