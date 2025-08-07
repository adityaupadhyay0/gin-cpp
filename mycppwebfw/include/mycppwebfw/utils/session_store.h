#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace mycppwebfw
{
namespace utils
{

class SessionStore
{
public:
    virtual ~SessionStore() = default;
    virtual void set(const std::string& session_id,
                     const std::string& data) = 0;
    virtual std::string get(const std::string& session_id) = 0;
    virtual void remove(const std::string& session_id) = 0;
};

class InMemorySessionStore : public SessionStore
{
public:
    void set(const std::string& session_id, const std::string& data) override;
    std::string get(const std::string& session_id) override;
    void remove(const std::string& session_id) override;

private:
    std::unordered_map<std::string, std::string> data_;
    std::mutex mutex_;
};

}  // namespace utils
}  // namespace mycppwebfw
