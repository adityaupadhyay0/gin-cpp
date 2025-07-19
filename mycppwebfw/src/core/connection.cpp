#include "mycppwebfw/core/connection.h"
#include "mycppwebfw/core/connection_manager.h"
#include <utility>
#include <vector>

namespace mycppwebfw {
namespace core {

Connection::Connection(asio::ip::tcp::socket socket, ConnectionManager& manager)
    : socket_(std::move(socket)),
      connection_manager_(manager) {
}

void Connection::start() {
    do_read();
}

void Connection::stop() {
    socket_.close();
}

void Connection::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(asio::buffer(buffer_),
        [this, self](std::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                http::Parser::ParseResult result;
                result = parser_.parse(
                    request_, buffer_.data(), buffer_.data() + bytes_transferred);

                if (result == http::Parser::ParseResult::GOOD) {
                    // For now, just send a simple response.
                    // Later, this will be handled by a request handler.
                    response_ = http::Response::stock_response(http::Response::StatusCode::ok);
                    do_write();
                } else if (result == http::Parser::ParseResult::BAD) {
                    response_ = http::Response::stock_response(http::Response::StatusCode::bad_request);
                    do_write();
                } else {
                    do_read();
                }
            } else if (ec != asio::error::operation_aborted) {
                connection_manager_.stop(shared_from_this());
            }
        });
}

void Connection::do_write() {
    auto self(shared_from_this());
    asio::async_write(socket_, response_.to_buffers(),
        [this, self](std::error_code ec, std::size_t) {
            if (!ec) {
                asio::error_code ignored_ec;
                socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
            }
            if (ec != asio::error::operation_aborted) {
                connection_manager_.stop(shared_from_this());
            }
        });
}

} // namespace core
} // namespace mycppwebfw
