#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>

namespace mycppwebfw
{
namespace logging
{

class LogSink
{
public:
    virtual ~LogSink() = default;
    virtual void submit(const std::string& message) = 0;
};

class ConsoleSink : public LogSink
{
public:
    void submit(const std::string& message) override;

private:
    std::mutex mutex_;
};

class FileSink : public LogSink
{
public:
    FileSink(const std::string& filepath);
    void submit(const std::string& message) override;

private:
    std::ofstream file_;
    std::mutex mutex_;
};

}  // namespace logging
}  // namespace mycppwebfw
