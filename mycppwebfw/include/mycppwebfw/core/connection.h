#pragma once

#include "asio.hpp"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/http/parser.h"
#include <memory>
#include "mycppwebfw/core/buffer_pool.h"

namespace mycppwebfw {
namespace core {

class ConnectionManager;

enum class ConnState {
    Idle,
    Reading,
    Writing,
    Closed
};

class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    explicit Connection(asio::ip::tcp::socket socket, ConnectionManager& manager);

    void start();
    void stop();

    // Keep-alive and timeout config
    void set_keep_alive(bool enabled);
    void set_idle_timeout(size_t ms);
    void set_read_timeout(size_t ms);
    void set_write_timeout(size_t ms);
    ConnState get_state() const;

private:
    void do_read();
    void do_write();
    void on_timeout(const asio::error_code& ec);
    void transition_state(ConnState new_state);
    void shutdown();
    void start_idle_timer();
    void start_read_timer();
    void start_write_timer();

    asio::ip::tcp::socket socket_;
    ConnectionManager& connection_manager_;

    std::shared_ptr<std::vector<char>> buffer_;
    static std::shared_ptr<BufferPool<>> buffer_pool_;

    http::Request request_;
    http::Response response_;
    http::Parser parser_;

    // Connection lifecycle fields
    bool keep_alive_ = false;
    size_t idle_timeout_ms_ = 60000;
    size_t read_timeout_ms_ = 30000;
    size_t write_timeout_ms_ = 30000;
    ConnState state_ = ConnState::Idle;
    asio::steady_timer timer_;
};

} // namespace core
} // namespace mycppwebfw
