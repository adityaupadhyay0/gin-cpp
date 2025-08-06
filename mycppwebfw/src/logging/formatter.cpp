#include "formatter.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace mycppwebfw
{
namespace logging
{

namespace
{
std::string to_string(const std::chrono::system_clock::time_point& tp)
{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return buf;
}
}  // namespace

std::string TextFormatter::format(const LogRecord& record)
{
    std::stringstream ss;
    ss << "[" << to_string(record.timestamp) << "] " << record.remote_ip
       << " \"" << record.method << " " << record.path << "\" "
       << record.status_code << " " << std::fixed << std::setprecision(6)
       << record.latency.count() << "s";
    return ss.str();
}

std::string JsonFormatter::format(const LogRecord& record)
{
    std::stringstream ss;
    ss << "{" << "\"timestamp\": \"" << to_string(record.timestamp) << "\", "
       << "\"remote_ip\": \"" << record.remote_ip << "\", " << "\"method\": \""
       << record.method << "\", " << "\"path\": \"" << record.path << "\", "
       << "\"status\": " << record.status_code << ", "
       << "\"latency_s\": " << std::fixed << std::setprecision(6)
       << record.latency.count() << "}";
    return ss.str();
}

}  // namespace logging
}  // namespace mycppwebfw
