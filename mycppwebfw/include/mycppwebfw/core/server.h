#pragma once

#include "asio.hpp"
#include "mycppwebfw/core/connection_manager.h"
#include <string>

namespace mycppwebfw {
namespace core {

class Server {
public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    explicit Server(const std::string& address, const std::string& port);

    void run();

private:
    void do_accept();
    void do_await_stop();

    asio::io_context io_context_;
    asio::signal_set signals_;
    asio::ip::tcp::acceptor acceptor_;
    ConnectionManager connection_manager_;
};

} // namespace core
} // namespace mycppwebfw
