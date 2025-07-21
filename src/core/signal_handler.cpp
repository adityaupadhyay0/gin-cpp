#include "mycppwebfw/core/signal_handler.h"
#include <iostream>

namespace mycppwebfw {
namespace core {

SignalHandler::SignalHandler(asio::io_context& io_context)
#ifdef _WIN32
    : signals_(io_context, SIGINT, SIGTERM) // SIGHUP not supported on Windows
#else
    : signals_(io_context, SIGINT, SIGTERM, SIGHUP)
#endif
{
}

void SignalHandler::register_handlers(const std::function<void(int)>& shutdown_handler,
                                      const std::function<void()>& reload_handler) {
    shutdown_handler_ = shutdown_handler;
    reload_handler_ = reload_handler;
    signals_.async_wait([this](const asio::error_code& ec, int signal_number) {
        handle_signal(ec, signal_number);
    });
}

void SignalHandler::handle_signal(const asio::error_code& ec, int signal_number) {
    if (!ec) {
        if (signal_number == SIGINT || signal_number == SIGTERM) {
            if (shutdown_handler_) shutdown_handler_(signal_number);
            std::cout << "Received shutdown signal: " << signal_number << std::endl;
        }
#ifndef _WIN32
        else if (signal_number == SIGHUP) {
            if (reload_handler_) reload_handler_();
            std::cout << "Received reload signal (SIGHUP)" << std::endl;
        }
#endif
        // Re-arm for future signals
        signals_.async_wait([this](const asio::error_code& ec, int signal_number) {
            handle_signal(ec, signal_number);
        });
    }
}

} // namespace core
} // namespace mycppwebfw
