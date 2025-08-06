#pragma once

#include <chrono>
#include <string>
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"

namespace mycppwebfw
{
namespace logging
{

struct LogRecord
{
    std::chrono::system_clock::time_point timestamp;
    std::string remote_ip;
    std::string method;
    std::string path;
    int status_code;
    std::chrono::duration<double> latency;
};

class Formatter
{
public:
    virtual ~Formatter() = default;
    virtual std::string format(const LogRecord& record) = 0;
};

class TextFormatter : public Formatter
{
public:
    std::string format(const LogRecord& record) override;
};

class JsonFormatter : public Formatter
{
public:
    std::string format(const LogRecord& record) override;
};

}  // namespace logging
}  // namespace mycppwebfw
