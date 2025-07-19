#pragma once

#include <string>
#include <chrono>
#include <cstdint>

namespace mycppwebfw {
namespace utils {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class ServerConfig {
public:
    ServerConfig();

    void load_from_file(const std::string& path);
    void load_from_env();
    void validate() const;

    // Getters
    uint16_t get_port() const;
    const std::string& get_bind_address() const;
    size_t get_max_connections() const;
    std::chrono::seconds get_connection_timeout() const;
    size_t get_thread_pool_size() const;
    bool is_ssl_enabled() const;
    LogLevel get_log_level() const;

private:
    uint16_t port_;
    std::string bind_address_;
    size_t max_connections_;
    std::chrono::seconds connection_timeout_;
    size_t thread_pool_size_;
    bool enable_ssl_;
    LogLevel log_level_;
};

} // namespace utils
} // namespace mycppwebfw
