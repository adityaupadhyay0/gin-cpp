#include "mycppwebfw/utils/session_store.h"

namespace mycppwebfw
{
namespace utils
{

void InMemorySessionStore::set(const std::string& session_id,
                               const std::string& data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    data_[session_id] = data;
}

std::string InMemorySessionStore::get(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(session_id);
    if (it != data_.end())
    {
        return it->second;
    }
    return "";
}

void InMemorySessionStore::remove(const std::string& session_id)
{
    std::lock_guard<std::mutex> lock(mutex_);
    data_.erase(session_id);
}

}  // namespace utils
}  // namespace mycppwebfw
