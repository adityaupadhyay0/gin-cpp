#include "mycppwebfw/middleware/logger.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include <chrono>

namespace mycppwebfw {
namespace middleware {

struct Logger::Impl {
    Impl(std::unique_ptr<logging::Formatter> formatter,
         std::unique_ptr<logging::LogSink> sink)
        : formatter_(std::move(formatter)), sink_(std::move(sink)) {}

    std::unique_ptr<logging::Formatter> formatter_;
    std::unique_ptr<logging::LogSink> sink_;
};

Logger::Logger(std::unique_ptr<logging::Formatter> formatter,
               std::unique_ptr<logging::LogSink> sink)
    : pimpl_(std::make_unique<Impl>(std::move(formatter), std::move(sink))) {}

Logger::~Logger() = default;

void Logger::operator()(Context& ctx, Next next) {
    auto& req = ctx.req();
    auto start = std::chrono::high_resolution_clock::now();
    next();
    auto& res = ctx.res();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto log_message = pimpl_->formatter_->format(req, res, duration);
    pimpl_->sink_->log(log_message);
}

} // namespace middleware
} // namespace mycppwebfw
