#pragma once
#include <iostream>
#include <string>

namespace mycppwebfw
{
namespace utils
{

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    static void log(LogLevel level, const std::string& message);
    static void log(LogLevel level, const std::string& message, const char* file, int line);
};
}  // namespace utils
}  // namespace mycppwebfw

// Simple logging macros for mycppwebfw
#define LOG_DEBUG(msg) mycppwebfw::utils::Logger::log(mycppwebfw::utils::LogLevel::DEBUG, msg, __FILE__, __LINE__)
#define LOG_INFO(msg) mycppwebfw::utils::Logger::log(mycppwebfw::utils::LogLevel::INFO, msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) mycppwebfw::utils::Logger::log(mycppwebfw::utils::LogLevel::WARNING, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) mycppwebfw::utils::Logger::log(mycppwebfw::utils::LogLevel::ERROR, msg, __FILE__, __LINE__)
