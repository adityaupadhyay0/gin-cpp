#pragma once

#include "middleware/logger.h"

namespace mycppwebfw
{
namespace middleware
{

struct Logger::Impl
{
    Impl(std::unique_ptr<logging::Formatter> formatter,
         std::unique_ptr<logging::LogSink> sink)
        : formatter_(std::move(formatter)), sink_(std::move(sink))
    {
    }

    std::unique_ptr<logging::Formatter> formatter_;
    std::unique_ptr<logging::LogSink> sink_;
};

}  // namespace middleware
}  // namespace mycppwebfw
