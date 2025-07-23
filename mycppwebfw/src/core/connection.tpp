#include "mycppwebfw/core/connection.h"
#include "mycppwebfw/core/connection_manager.h"
#include <utility>
#include <vector>
#include <iostream>

namespace mycppwebfw {
namespace core {

template <typename SocketType>
std::shared_ptr<BufferPool<>> Connection<SocketType>::buffer_pool_ = std::make_shared<BufferPool<>>(8192, 128);

template <typename SocketType>
Connection<SocketType>::Connection(SocketType socket, ConnectionManager& manager)
    : socket_(std::move(socket)),
      connection_manager_(manager),
      timer_(socket_.get_executor()) {
    buffer_ = buffer_pool_->acquire();
}

template <typename SocketType>
void Connection<SocketType>::start() {
    transition_state(ConnState::Reading);
    do_read();
}

template <typename SocketType>
void Connection<SocketType>::stop() {
    shutdown();
}

template <typename SocketType>
void Connection<SocketType>::shutdown() {
    transition_state(ConnState::Closed);
    asio::error_code ec;
    timer_.cancel();
    socket_.close(ec);
    if (buffer_) {
        buffer_pool_->release(buffer_);
        buffer_.reset();
    }
}

template <typename SocketType>
void Connection<SocketType>::do_read() {
    auto self(this->shared_from_this());
    transition_state(ConnState::Reading);
    start_read_timer();
    socket_.async_read_some(asio::buffer(*buffer_),
        [this, self](std::error_code ec, std::size_t bytes_transferred) {
            timer_.cancel();
            if (!ec) {
                http::Parser::ParseResult result;
                result = parser_.parse(
                    request_, buffer_->data(), buffer_->data() + bytes_transferred);

                // Keep-alive header handling
                auto conn_hdr = std::find_if(request_.headers.begin(), request_.headers.end(),
                    [](const http::Header& h) { return h.name == "Connection"; });
                if (conn_hdr != request_.headers.end()) {
                    std::string val = conn_hdr->value;
                    for (auto& c : val) c = tolower(c);
                    keep_alive_ = (val.find("keep-alive") != std::string::npos);
                } else {
                    keep_alive_ = (request_.http_version_major == 1 && request_.http_version_minor == 1); // Default for HTTP/1.1
                }

                if (result == http::Parser::ParseResult::GOOD) {
                    // Validate protocol compliance
                    bool valid = true;
                    if ((request_.http_version_major != 1 || request_.http_version_minor != 1) &&
                        (request_.http_version_major != 1 || request_.http_version_minor != 0)) {
                        valid = false;
                    }
                    if (request_.method.empty() || request_.uri.empty()) {
                        valid = false;
                    }
                    if (request_.http_version_major == 1 && request_.http_version_minor == 1) {
                        auto host_hdr = std::find_if(request_.headers.begin(), request_.headers.end(),
                            [](const http::Header& h) { return h.name == "Host"; });
                        if (host_hdr == request_.headers.end()) {
                            valid = false;
                        }
                    }
                    if (!valid) {
                        response_ = http::Response::stock_response(http::Response::StatusCode::bad_request);
                    } else {
                        response_ = http::Response::stock_response(http::Response::StatusCode::ok);
                    }
                    do_write();
                } else if (result == http::Parser::ParseResult::BAD) {
                    response_ = http::Response::stock_response(http::Response::StatusCode::bad_request);
                    do_write();
                } else {
                    do_read();
                }
            } else if (ec != asio::error::operation_aborted) {
                std::cerr << "Socket read error: " << ec.message() << std::endl;
                response_ = http::Response::stock_response(http::Response::StatusCode::internal_server_error);
                do_write();
                connection_manager_.stop(this->shared_from_this());
            }
        });
}

template <typename SocketType>
void Connection<SocketType>::do_write() {
    auto self(this->shared_from_this());
    transition_state(ConnState::Writing);
    start_write_timer();
    asio::async_write(socket_, response_.to_buffers(),
        [this, self](std::error_code ec, std::size_t) {
            timer_.cancel();
            if (!ec) {
                if (keep_alive_) {
                    transition_state(ConnState::Idle);
                    start_idle_timer();
                    do_read(); // Reuse connection for next request
                } else {
                    asio::error_code ignored_ec;
                    socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
                    connection_manager_.stop(this->shared_from_this());
                }
            } else {
                std::cerr << "Socket write error: " << ec.message() << std::endl;
                connection_manager_.stop(this->shared_from_this());
            }
        });
}

// --- Timeout and state helpers ---
template <typename SocketType>
void Connection<SocketType>::start_idle_timer() {
    timer_.expires_after(std::chrono::milliseconds(idle_timeout_ms_));
    auto self(this->shared_from_this());
    timer_.async_wait([this, self](const asio::error_code& ec) {
        if (!ec && state_ == ConnState::Idle) {
            std::cerr << "Idle timeout, closing connection." << std::endl;
            shutdown();
            connection_manager_.stop(self);
        }
    });
}

template <typename SocketType>
void Connection<SocketType>::start_read_timer() {
    timer_.expires_after(std::chrono::milliseconds(read_timeout_ms_));
    auto self(this->shared_from_this());
    timer_.async_wait([this, self](const asio::error_code& ec) {
        if (!ec && state_ == ConnState::Reading) {
            std::cerr << "Read timeout, closing connection." << std::endl;
            shutdown();
            connection_manager_.stop(self);
        }
    });
}

template <typename SocketType>
void Connection<SocketType>::start_write_timer() {
    timer_.expires_after(std::chrono::milliseconds(write_timeout_ms_));
    auto self(this->shared_from_this());
    timer_.async_wait([this, self](const asio::error_code& ec) {
        if (!ec && state_ == ConnState::Writing) {
            std::cerr << "Write timeout, closing connection." << std::endl;
            shutdown();
            connection_manager_.stop(self);
        }
    });
}

template <typename SocketType>
void Connection<SocketType>::transition_state(ConnState new_state) {
    state_ = new_state;
}

template <typename SocketType>
ConnState Connection<SocketType>::get_state() const {
    return state_;
}

template <typename SocketType>
void Connection<SocketType>::set_keep_alive(bool enabled) {
    keep_alive_ = enabled;
}

template <typename SocketType>
void Connection<SocketType>::set_idle_timeout(size_t ms) {
    idle_timeout_ms_ = ms;
}

template <typename SocketType>
void Connection<SocketType>::set_read_timeout(size_t ms) {
    read_timeout_ms_ = ms;
}

template <typename SocketType>
void Connection<SocketType>::set_write_timeout(size_t ms) {
    write_timeout_ms_ = ms;
}

} // namespace core
} // namespace mycppwebfw
