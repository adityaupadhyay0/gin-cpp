#pragma once
#include <functional>
#include <asio/signal_set.hpp>
#include <asio/io_context.hpp>

namespace mycppwebfw {
namespace core {

class SignalHandler {
public:
    SignalHandler(asio::io_context& io_context);
    // Register shutdown and reload handlers
    void register_handlers(const std::function<void(int)>& shutdown_handler,
                          const std::function<void()>& reload_handler = nullptr);
private:
    asio::signal_set signals_;
    std::function<void(int)> shutdown_handler_;
    std::function<void()> reload_handler_;
    void handle_signal(const asio::error_code& ec, int signal_number);
};

} // namespace core
} // namespace mycppwebfw
