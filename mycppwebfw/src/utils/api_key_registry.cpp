#include "mycppwebfw/utils/api_key_registry.h"

namespace mycppwebfw {
namespace utils {

void ApiKeyRegistry::add_key(const std::string& key, const std::string& user) {
    keys_[key] = user;
}

bool ApiKeyRegistry::is_valid(const std::string& key, std::string& user) {
    auto it = keys_.find(key);
    if (it != keys_.end()) {
        user = it->second;
        return true;
    }
    return false;
}

} // namespace utils
} // namespace mycppwebfw
