#include <gtest/gtest.h>
#include "mycppwebfw/core/connection.h"
#include "asio.hpp"
#include <memory>
#include <thread>

using namespace mycppwebfw::core;

class DummyManager : public ConnectionManager {
    // Minimal stub for testing
};

TEST(ConnectionLifecycle, KeepAliveFlag) {
    asio::io_context io;
    DummyManager mgr;
    asio::ip::tcp::socket sock(io);
    auto conn = std::make_shared<Connection>(std::move(sock), mgr);
    conn->set_keep_alive(true);
    EXPECT_TRUE(conn->get_state() == ConnState::Idle || conn->get_state() == ConnState::Closed);
}

TEST(ConnectionLifecycle, TimeoutSettings) {
    asio::io_context io;
    DummyManager mgr;
    asio::ip::tcp::socket sock(io);
    auto conn = std::make_shared<Connection>(std::move(sock), mgr);
    conn->set_idle_timeout(100);
    conn->set_read_timeout(100);
    conn->set_write_timeout(100);
    EXPECT_EQ(conn->get_state(), ConnState::Idle);
}

TEST(ConnectionLifecycle, StateTransitions) {
    asio::io_context io;
    DummyManager mgr;
    asio::ip::tcp::socket sock(io);
    auto conn = std::make_shared<Connection>(std::move(sock), mgr);
    conn->start();
    EXPECT_EQ(conn->get_state(), ConnState::Reading);
    conn->stop();
    EXPECT_EQ(conn->get_state(), ConnState::Closed);
}

TEST(ConnectionLifecycle, IdleTimeoutClosesConnection) {
    asio::io_context io;
    DummyManager mgr;
    asio::ip::tcp::socket sock(io);
    auto conn = std::make_shared<Connection>(std::move(sock), mgr);
    conn->set_idle_timeout(1); // 1ms for fast test
    conn->start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_EQ(conn->get_state(), ConnState::Closed);
}

TEST(ConnectionLifecycle, AbruptDisconnectCleansUp) {
    asio::io_context io;
    DummyManager mgr;
    asio::ip::tcp::socket sock(io);
    auto conn = std::make_shared<Connection>(std::move(sock), mgr);
    conn->start();
    conn->stop(); // Simulate abrupt disconnect
    EXPECT_EQ(conn->get_state(), ConnState::Closed);
}

TEST(ConnectionLifecycle, MultipleRequestsWithKeepAlive) {
    asio::io_context io;
    DummyManager mgr;
    asio::ip::tcp::socket sock(io);
    auto conn = std::make_shared<Connection>(std::move(sock), mgr);
    conn->set_keep_alive(true);
    conn->start();
    // Simulate multiple requests by calling do_read and do_write
    conn->do_read();
    conn->do_write();
    conn->do_read();
    conn->do_write();
    EXPECT_TRUE(conn->get_state() == ConnState::Idle || conn->get_state() == ConnState::Reading);
}

// More tests can be added for edge cases, e.g., timeouts, abrupt disconnects, multiple requests, etc.
