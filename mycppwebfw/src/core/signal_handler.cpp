#include "mycppwebfw/core/signal_handler.h"
#include <signal.h>

namespace mycppwebfw {
namespace core {

SignalHandler::SignalHandler(asio::io_context& io_context)
    : signals_(io_context) {
}

void SignalHandler::register_handlers(std::function<void(int)> shutdown_callback, std::function<void()> reload_callback) {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#ifdef SIGQUIT
    signals_.add(SIGQUIT);
#endif // SIGQUIT
#ifdef SIGHUP
    signals_.add(SIGHUP);
#endif // SIGHUP

    signals_.async_wait(
        [shutdown_callback, reload_callback](const asio::error_code&, int signo) {
            if (signo == SIGHUP) {
                reload_callback();
            } else {
                shutdown_callback(signo);
            }
        });
}

} // namespace core
} // namespace mycppwebfw
