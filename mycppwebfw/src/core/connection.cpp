#include "mycppwebfw/core/connection.h"
#include "mycppwebfw/core/connection_manager.h"
#include <utility>
#include <vector>

namespace mycppwebfw {
namespace core {


std::shared_ptr<BufferPool> Connection::buffer_pool_ = std::make_shared<BufferPool>(8192, 128);

Connection::Connection(asio::ip::tcp::socket socket, ConnectionManager& manager)
    : socket_(std::move(socket)),
      connection_manager_(manager),
      timer_(socket_.get_executor()) {
    buffer_ = buffer_pool_->acquire();
}

void Connection::start() {
    transition_state(ConnState::Reading);
    start_idle_timer();
    do_read();
}


void Connection::stop() {
    shutdown();
}

void Connection::shutdown() {
    transition_state(ConnState::Closed);
    asio::error_code ec;
    timer_.cancel();
    socket_.close(ec);
    if (buffer_) {
        buffer_pool_->release(buffer_);
        buffer_.reset();
    }
}


void Connection::do_read() {
    auto self(shared_from_this());
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
                auto conn_hdr = request_.headers.find("Connection");
                if (conn_hdr != request_.headers.end()) {
                    std::string val = conn_hdr->second;
                    for (auto& c : val) c = tolower(c);
                    keep_alive_ = (val.find("keep-alive") != std::string::npos);
                } else {
                    keep_alive_ = (request_.http_version == "1.1"); // Default for HTTP/1.1
                }

                if (result == http::Parser::ParseResult::GOOD) {
                    // Validate protocol compliance
                    bool valid = true;
                    if (request_.http_version != "1.1" && request_.http_version != "1.0") {
                        valid = false;
                    }
                    if (request_.method.empty() || request_.uri.empty()) {
                        valid = false;
                    }
                    if (request_.http_version == "1.1" && request_.headers.find("Host") == request_.headers.end()) {
                        valid = false;
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
                connection_manager_.stop(shared_from_this());
            }
        });
}

void Connection::do_write() {
    auto self(shared_from_this());
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
                    connection_manager_.stop(shared_from_this());
                }
            } else {
                std::cerr << "Socket write error: " << ec.message() << std::endl;
                connection_manager_.stop(shared_from_this());
            }
        });
}

// --- Timeout and state helpers ---
void Connection::start_idle_timer() {
    timer_.expires_after(std::chrono::milliseconds(idle_timeout_ms_));
    auto self(shared_from_this());
    timer_.async_wait([this, self](const asio::error_code& ec) {
        if (!ec && state_ == ConnState::Idle) {
            std::cerr << "Idle timeout, closing connection." << std::endl;
            shutdown();
            connection_manager_.stop(self);
        }
    });
}

void Connection::start_read_timer() {
    timer_.expires_after(std::chrono::milliseconds(read_timeout_ms_));
    auto self(shared_from_this());
    timer_.async_wait([this, self](const asio::error_code& ec) {
        if (!ec && state_ == ConnState::Reading) {
            std::cerr << "Read timeout, closing connection." << std::endl;
            shutdown();
            connection_manager_.stop(self);
        }
    });
}

void Connection::start_write_timer() {
    timer_.expires_after(std::chrono::milliseconds(write_timeout_ms_));
    auto self(shared_from_this());
    timer_.async_wait([this, self](const asio::error_code& ec) {
        if (!ec && state_ == ConnState::Writing) {
            std::cerr << "Write timeout, closing connection." << std::endl;
            shutdown();
            connection_manager_.stop(self);
        }
    });
}

void Connection::transition_state(ConnState new_state) {
    state_ = new_state;
}

ConnState Connection::get_state() const {
    return state_;
}

void Connection::set_keep_alive(bool enabled) {
    keep_alive_ = enabled;
}

void Connection::set_idle_timeout(size_t ms) {
    idle_timeout_ms_ = ms;
}

void Connection::set_read_timeout(size_t ms) {
    read_timeout_ms_ = ms;
}

void Connection::set_write_timeout(size_t ms) {
    write_timeout_ms_ = ms;
}

} // namespace core
} // namespace mycppwebfw
