#include "utils/logger.h"
#include <iostream>

namespace mycppwebfw {
namespace utils {

void Logger::log(LogLevel level, const std::string& message) {
    std::string level_str;
    switch (level) {
        case LogLevel::DEBUG:
            level_str = "DEBUG";
            break;
        case LogLevel::INFO:
            level_str = "INFO";
            break;
        case LogLevel::WARNING:
            level_str = "WARNING";
            break;
        case LogLevel::ERROR:
            level_str = "ERROR";
            break;
    }
    std::cout << "[" << level_str << "] " << message << std::endl;
}

} // namespace utils
} // namespace mycppwebfw
