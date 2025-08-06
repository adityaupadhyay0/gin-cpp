#pragma once

#include <memory>
#include "logging/formatter.h"
#include "logging/sinks.h"
#include "middleware.h"

namespace mycppwebfw
{
namespace middleware
{

class Logger : public Middleware
{
public:
    Logger(std::unique_ptr<logging::Formatter> formatter,
           std::unique_ptr<logging::LogSink> sink);
    ~Logger();

    void operator()(const Request& req, Response& res,
                    std::function<void()> next) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

}  // namespace middleware
}  // namespace mycppwebfw
