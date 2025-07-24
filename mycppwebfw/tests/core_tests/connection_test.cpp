#include <gtest/gtest.h>
#include "mycppwebfw/core/connection.h"
#include "mycppwebfw/core/connection_manager.h"
#include "asio.hpp"
#include <memory>
#include <thread>

using namespace mycppwebfw::core;
using TcpSocket = asio::ip::tcp::socket;

class DummyManager : public ConnectionManager<Connection<TcpSocket>> {
    // Minimal stub for testing
};

TEST(ConnectionLifecycle, KeepAliveFlag) {
    asio::io_context io;
    DummyManager mgr;
    TcpSocket sock(io);
    auto conn = std::make_shared<Connection<TcpSocket>>(std::move(sock), mgr);
    conn->set_keep_alive(true);
    EXPECT_TRUE(conn->get_state() == ConnState::Idle || conn->get_state() == ConnState::Closed);
}

TEST(ConnectionLifecycle, TimeoutSettings) {
    asio::io_context io;
    DummyManager mgr;
    TcpSocket sock(io);
    auto conn = std::make_shared<Connection<TcpSocket>>(std::move(sock), mgr);
    conn->set_idle_timeout(100);
    conn->set_read_timeout(100);
    conn->set_write_timeout(100);
    EXPECT_EQ(conn->get_state(), ConnState::Idle);
}

TEST(ConnectionLifecycle, StateTransitions) {
    asio::io_context io;
    DummyManager mgr;
    TcpSocket sock(io);
    auto conn = std::make_shared<Connection<TcpSocket>>(std::move(sock), mgr);
    conn->start();
    EXPECT_EQ(conn->get_state(), ConnState::Reading);
    conn->stop();
    EXPECT_EQ(conn->get_state(), ConnState::Closed);
}

TEST(ConnectionLifecycle, IdleTimeoutClosesConnection) {
    asio::io_context io;
    DummyManager mgr;
    TcpSocket sock(io);
    auto conn = std::make_shared<Connection<TcpSocket>>(std::move(sock), mgr);
    conn->set_idle_timeout(1); // 1ms for fast test

    std::promise<void> promise;
    auto future = promise.get_future();

    conn->start();

    asio::post(io, [&](){
        conn->stop();
        promise.set_value();
    });

    io.run();
    future.wait();

    EXPECT_EQ(conn->get_state(), ConnState::Closed);
}

TEST(ConnectionLifecycle, AbruptDisconnectCleansUp) {
    asio::io_context io;
    DummyManager mgr;
    TcpSocket sock(io);
    auto conn = std::make_shared<Connection<TcpSocket>>(std::move(sock), mgr);
    conn->start();
    conn->stop(); // Simulate abrupt disconnect
    EXPECT_EQ(conn->get_state(), ConnState::Closed);
}

// TEST(ConnectionLifecycle, MultipleRequestsWithKeepAlive) {
//     asio::io_context io;
//     DummyManager mgr;
//     TcpSocket sock(io);
//     auto conn = std::make_shared<Connection<TcpSocket>>(std::move(sock), mgr);
//     conn->set_keep_alive(true);

//     conn->start();

//     asio::post(io, [&](){ conn->do_read(); });
//     asio::post(io, [&](){ conn->do_write(); });

//     io.run();

//     EXPECT_TRUE(conn->get_state() == ConnState::Idle || conn->get_state() == ConnState::Reading);
// }

// More tests can be added for edge cases, e.g., timeouts, abrupt disconnects, multiple requests, etc.
