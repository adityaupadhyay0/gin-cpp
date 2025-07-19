#include "mycppwebfw/utils/config.h"
#include <cstdlib>
#include <stdexcept>
#include "nlohmann/json.hpp"
#include <fstream>

namespace mycppwebfw {
namespace utils {

using json = nlohmann::json;

ServerConfig::ServerConfig()
    : port_(8080),
      bind_address_("0.0.0.0"),
      max_connections_(1024),
      connection_timeout_(60),
      thread_pool_size_(4),
      enable_ssl_(false),
      log_level_(LogLevel::INFO) {}

void ServerConfig::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + path);
    }
    json config_json;
    file >> config_json;

    port_ = config_json.value("port", port_);
    bind_address_ = config_json.value("bind_address", bind_address_);
    max_connections_ = config_json.value("max_connections", max_connections_);
    connection_timeout_ = std::chrono::seconds(config_json.value("connection_timeout", connection_timeout_.count()));
    thread_pool_size_ = config_json.value("thread_pool_size", thread_pool_size_);
    enable_ssl_ = config_json.value("enable_ssl", enable_ssl_);

    std::string log_level_str = config_json.value("log_level", "INFO");
    if (log_level_str == "DEBUG") {
        log_level_ = LogLevel::DEBUG;
    } else if (log_level_str == "WARN") {
        log_level_ = LogLevel::WARN;
    } else if (log_level_str == "ERROR") {
        log_level_ = LogLevel::ERROR;
    } else {
        log_level_ = LogLevel::INFO;
    }
}

void ServerConfig::load_from_env() {
    if (const char* port_env = std::getenv("SERVER_PORT")) {
        port_ = std::stoi(port_env);
    }
    if (const char* bind_address_env = std::getenv("SERVER_BIND_ADDRESS")) {
        bind_address_ = bind_address_env;
    }
    if (const char* max_connections_env = std::getenv("SERVER_MAX_CONNECTIONS")) {
        max_connections_ = std::stoul(max_connections_env);
    }
    if (const char* connection_timeout_env = std::getenv("SERVER_CONNECTION_TIMEOUT")) {
        connection_timeout_ = std::chrono::seconds(std::stoul(connection_timeout_env));
    }
    if (const char* thread_pool_size_env = std::getenv("SERVER_THREAD_POOL_SIZE")) {
        thread_pool_size_ = std::stoul(thread_pool_size_env);
    }
    if (const char* enable_ssl_env = std::getenv("SERVER_ENABLE_SSL")) {
        enable_ssl_ = (std::string(enable_ssl_env) == "true");
    }
    if (const char* log_level_env = std::getenv("SERVER_LOG_LEVEL")) {
        std::string log_level_str(log_level_env);
        if (log_level_str == "DEBUG") {
            log_level_ = LogLevel::DEBUG;
        } else if (log_level_str == "WARN") {
            log_level_ = LogLevel::WARN;
        } else if (log_level_str == "ERROR") {
            log_level_ = LogLevel::ERROR;
        } else {
            log_level_ = LogLevel::INFO;
        }
    }
}

void ServerConfig::validate() const {
    if (port_ == 0 || port_ > 65535) {
        throw std::invalid_argument("Invalid port number");
    }
    if (max_connections_ == 0) {
        throw std::invalid_argument("Max connections must be greater than 0");
    }
    if (thread_pool_size_ == 0) {
        throw std::invalid_argument("Thread pool size must be greater than 0");
    }
}

uint16_t ServerConfig::get_port() const { return port_; }
const std::string& ServerConfig::get_bind_address() const { return bind_address_; }
size_t ServerConfig::get_max_connections() const { return max_connections_; }
std::chrono::seconds ServerConfig::get_connection_timeout() const { return connection_timeout_; }
size_t ServerConfig::get_thread_pool_size() const { return thread_pool_size_; }
bool ServerConfig::is_ssl_enabled() const { return enable_ssl_; }
LogLevel ServerConfig::get_log_level() const { return log_level_; }

} // namespace utils
} // namespace mycppwebfw
