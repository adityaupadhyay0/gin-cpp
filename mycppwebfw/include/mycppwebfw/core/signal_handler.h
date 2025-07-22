#pragma once

#include "asio.hpp"
#include <functional>

namespace mycppwebfw {
namespace core {

class SignalHandler {
public:
    SignalHandler(asio::io_context& io_context);
    void register_handlers(std::function<void(int)> shutdown_callback, std::function<void()> reload_callback);

private:
    asio::signal_set signals_;
};

} // namespace core
} // namespace mycppwebfw
