#include "gtest/gtest.h"
#include "mycppwebfw/http/response.h"

TEST(ResponseTest, StockResponse) {
    mycppwebfw::http::Response res = mycppwebfw::http::Response::stock_response(mycppwebfw::http::Response::StatusCode::not_found);

    EXPECT_EQ(res.status, mycppwebfw::http::Response::StatusCode::not_found);
    EXPECT_EQ(res.content, "");
    EXPECT_EQ(res.headers.size(), 2);
    EXPECT_EQ(res.headers[0].name, "Content-Length");
    EXPECT_EQ(res.headers[0].value, "0");
    EXPECT_EQ(res.headers[1].name, "Content-Type");
    EXPECT_EQ(res.headers[1].value, "text/html");
}

TEST(ResponseTest, ToBuffers) {
    mycppwebfw::http::Response res;
    res.status = mycppwebfw::http::Response::StatusCode::ok;
    res.content = "Hello, World!";
    res.headers.resize(2);
    res.headers[0].name = "Content-Length";
    res.headers[0].value = "13";
    res.headers[1].name = "Content-Type";
    res.headers[1].value = "text/plain";

    std::vector<asio::const_buffer> buffers = res.to_buffers();

    // Check the status line
    std::string status_line(asio::buffer_cast<const char*>(buffers[0]), asio::buffer_size(buffers[0]));
    EXPECT_EQ(status_line, "HTTP/1.0 200 OK\r\n");

    // Check the headers
    std::string header1_name(asio::buffer_cast<const char*>(buffers[1]), asio::buffer_size(buffers[1]));
    EXPECT_EQ(header1_name, "Content-Length");
    std::string header1_sep(asio::buffer_cast<const char*>(buffers[2]), asio::buffer_size(buffers[2]));
    EXPECT_EQ(header1_sep, ": ");
    std::string header1_value(asio::buffer_cast<const char*>(buffers[3]), asio::buffer_size(buffers[3]));
    EXPECT_EQ(header1_value, "13");
    std::string header1_crlf(asio::buffer_cast<const char*>(buffers[4]), asio::buffer_size(buffers[4]));
    EXPECT_EQ(header1_crlf, "\r\n");

    std::string header2_name(asio::buffer_cast<const char*>(buffers[5]), asio::buffer_size(buffers[5]));
    EXPECT_EQ(header2_name, "Content-Type");
    std::string header2_sep(asio::buffer_cast<const char*>(buffers[6]), asio::buffer_size(buffers[6]));
    EXPECT_EQ(header2_sep, ": ");
    std::string header2_value(asio::buffer_cast<const char*>(buffers[7]), asio::buffer_size(buffers[7]));
    EXPECT_EQ(header2_value, "text/plain");
    std::string header2_crlf(asio::buffer_cast<const char*>(buffers[8]), asio::buffer_size(buffers[8]));
    EXPECT_EQ(header2_crlf, "\r\n");

    // Check the final CRLF
    std::string final_crlf(asio::buffer_cast<const char*>(buffers[9]), asio::buffer_size(buffers[9]));
    EXPECT_EQ(final_crlf, "\r\n");

    // Check the content
    std::string content(asio::buffer_cast<const char*>(buffers[10]), asio::buffer_size(buffers[10]));
    EXPECT_EQ(content, "Hello, World!");
}
