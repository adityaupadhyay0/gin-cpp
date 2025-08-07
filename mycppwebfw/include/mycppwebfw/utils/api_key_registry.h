#pragma once

#include <string>
#include <unordered_map>

namespace mycppwebfw {
namespace utils {

class ApiKeyRegistry {
public:
    void add_key(const std::string& key, const std::string& user);
    bool is_valid(const std::string& key, std::string& user);

private:
    std::unordered_map<std::string, std::string> keys_;
};

} // namespace utils
} // namespace mycppwebfw
