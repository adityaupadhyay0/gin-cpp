#pragma once

#include "asio.hpp"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/http/parser.h"
#include <memory>

namespace mycppwebfw {
namespace core {

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    explicit Connection(asio::ip::tcp::socket socket, ConnectionManager& manager);

    void start();
    void stop();

private:
    void do_read();
    void do_write();

    asio::ip::tcp::socket socket_;
    ConnectionManager& connection_manager_;

    std::array<char, 8192> buffer_;

    http::Request request_;
    http::Response response_;
    http::Parser parser_;
};

} // namespace core
} // namespace mycppwebfw
