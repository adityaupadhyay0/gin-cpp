#include "mycppwebfw/utils/logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace mycppwebfw {
namespace utils {

static std::string levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
    }
    return "";
}

void Logger::log(LogLevel level, const std::string& message) {
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    // Format timestamp
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");

    // Output log message
    std::cout << "[" << ss.str() << "] "
              << "[" << levelToString(level) << "] "
              << message << std::endl;
}

void Logger::log(LogLevel level, const std::string& message, const char* file, int line) {
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    // Format timestamp
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");

    // Output log message with file and line info
    std::cout << "[" << ss.str() << "] "
              << "[" << levelToString(level) << "] "
              << "[" << file << ":" << line << "] "
              << message << std::endl;
}

} // namespace utils
} // namespace mycppwebfw
