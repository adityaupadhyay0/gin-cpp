#pragma once

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
};

}  // namespace utils
}  // namespace mycppwebfw
