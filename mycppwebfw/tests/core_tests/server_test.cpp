#include "gtest/gtest.h"
#include "mycppwebfw/core/server.h"
#include <thread>
#include "asio.hpp"

class ServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Run the server in a separate thread
        server_thread_ = std::thread([this]() {
            try {
                mycppwebfw::core::Server s("127.0.0.1", "8080");
                s.run();
            } catch (const std::exception& e) {
                FAIL() << "Server thread caught exception: " << e.what();
            }
        });

        // Give the server a moment to start up
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void TearDown() override {
        // The server is stopped by the test, so we just need to join the thread
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }

    std::thread server_thread_;
};

TEST_F(ServerTest, SignalHandlingShutdown) {
    // Simulate SIGINT and verify server stops gracefully
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(raise(SIGINT));
    // Server thread should join in TearDown
}

TEST_F(ServerTest, SimpleRequest) {
    asio::io_context io_context;
    asio::ip::tcp::socket socket(io_context);
    asio::ip::tcp::resolver resolver(io_context);

    try {
        asio::connect(socket, resolver.resolve("127.0.0.1", "8080"));

        std::string request = "GET /hello HTTP/1.1\r\n\r\n";
        asio::write(socket, asio::buffer(request));

        asio::streambuf response_buf;
        asio::read_until(socket, response_buf, "\r\n\r\n");

        std::istream response_stream(&response_buf);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;

        EXPECT_EQ(http_version, "HTTP/1.0");
        EXPECT_EQ(status_code, 200);

        // Stop the server by sending a signal
        raise(SIGINT);

    } catch (const std::exception& e) {
        FAIL() << "Test client caught exception: " << e.what();
    }
}
