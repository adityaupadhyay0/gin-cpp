#include "mycppwebfw/core/server.h"
#include "mycppwebfw/core/connection.h"
#include "mycppwebfw/core/connection.h"
#include <signal.h>
#include <iostream>
#include <utility>

namespace mycppwebfw {
namespace core {

Server::Server(const std::string& address, const std::string& port)
    : io_context_(1),
      acceptor_(io_context_),
      connection_manager_() {
    // Example thread pool config (could be loaded from config file)
    thread_pool_config_ = ThreadPoolConfig{};
    thread_pool_ = std::make_unique<ThreadPool>(thread_pool_config_);

    // Integrate SignalHandler for async signal handling and config reload

    signal_handler_ = std::make_unique<SignalHandler>(io_context_);
    signal_handler_->register_handlers(
        [this](int signo) { graceful_shutdown(signo); },
        [this]() { reload_config(); }
    );

    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    asio::ip::tcp::resolver resolver(io_context_);
    asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    do_accept();
}

void Server::run() {
    io_context_.run();
}

void Server::do_accept() {
    acceptor_.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!acceptor_.is_open()) {
                return;
            }

            if (!ec) {
                connection_manager_.start(std::make_shared<TcpSocketConnection>(
                    std::move(socket), connection_manager_));
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }

            do_accept();
        });
}

void Server::graceful_shutdown(int signo) {
    std::cout << "Graceful shutdown triggered by signal: " << signo << std::endl;
    acceptor_.close();
    connection_manager_.stop_all();
    if (thread_pool_) {
        // Stop thread pool and join threads
        thread_pool_.reset();
    }
    // Add other cleanup hooks here if needed
}

void Server::reload_config() {
    if (buffer_pool_) {
        // Release all buffers and clear pool
        // (BufferPool handles its own cleanup, but you can force clear if needed)
        buffer_pool_.reset();
    }
    std::cout << "Reloading configuration..." << std::endl;
    // Example: reload thread pool config (should be thread-safe)
    // ThreadPoolConfig new_config = load_config_from_file();
    // thread_pool_->resize(new_config.max_threads);
    // Add logic to reload other runtime configs as needed
}

} // namespace core
} // namespace mycppwebfw
