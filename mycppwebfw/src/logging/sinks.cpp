#include "sinks.h"
#include <iostream>

namespace mycppwebfw
{
namespace logging
{

void ConsoleSink::submit(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << message << std::endl;
}

FileSink::FileSink(const std::string& filepath) : file_(filepath, std::ios::app)
{
    if (!file_.is_open())
    {
        throw std::runtime_error("Failed to open log file: " + filepath);
    }
}

void FileSink::submit(const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    file_ << message << std::endl;
}

}  // namespace logging
}  // namespace mycppwebfw
