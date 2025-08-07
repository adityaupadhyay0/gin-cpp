#pragma once

#include "mycppwebfw/middleware/base_middleware.h"
#include "mycppwebfw/logging/formatter.h"
#include "mycppwebfw/logging/sinks.h"
#include <memory>

namespace mycppwebfw {
namespace middleware {

class Logger : public IMiddleware {
public:
    Logger(std::unique_ptr<logging::Formatter> formatter,
           std::unique_ptr<logging::LogSink> sink);
    ~Logger() override;

    void operator()(Context& ctx, Next next) override;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace middleware
} // namespace mycppwebfw
