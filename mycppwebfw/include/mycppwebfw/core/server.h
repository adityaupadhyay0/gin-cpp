#pragma once

#include "asio.hpp"
#include "mycppwebfw/core/connection_manager.h"
#include "mycppwebfw/core/signal_handler.h"
#include "mycppwebfw/core/thread_pool.h"
#include "mycppwebfw/core/buffer_pool.h"
#include <string>

namespace mycppwebfw {
namespace core {

using TcpSocketConnection = Connection<asio::ip::tcp::socket>;

class Server {
public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    explicit Server(const std::string& address, const std::string& port);

    void run();

    void do_accept();
    void graceful_shutdown(int signo);
    void reload_config();

    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_;
    ConnectionManager<TcpSocketConnection> connection_manager_;
    std::unique_ptr<SignalHandler> signal_handler_;
    std::unique_ptr<ThreadPool> thread_pool_;
    ThreadPoolConfig thread_pool_config_;
    std::shared_ptr<BufferPool<>> buffer_pool_;
};

} // namespace core
} // namespace mycppwebfw
