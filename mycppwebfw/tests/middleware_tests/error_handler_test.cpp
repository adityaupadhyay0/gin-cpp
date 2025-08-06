#include "mycppwebfw/middleware/error_handler.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <stdexcept>
#include "mycppwebfw/http/header.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"

TEST(ErrorHandlerTest, CatchesStdExceptionAndReturns500)
{
    auto error_handler = mycppwebfw::middleware::create_error_handler();
    mycppwebfw::http::Request req;
    mycppwebfw::http::Response res;
    auto next = []() { throw std::runtime_error("Something went wrong"); };

    error_handler.handler(req, res, next);

    EXPECT_EQ(res.status,
              mycppwebfw::http::Response::StatusCode::internal_server_error);
}

TEST(ErrorHandlerTest, ReturnsHtmlError)
{
    auto error_handler = mycppwebfw::middleware::create_error_handler();
    mycppwebfw::http::Request req;
    req.headers.push_back({"Accept", "text/html"});
    mycppwebfw::http::Response res;
    auto next = []() { throw std::runtime_error("Test error"); };

    error_handler.handler(req, res, next);

    EXPECT_EQ(res.status,
              mycppwebfw::http::Response::StatusCode::internal_server_error);
    auto content_type_it = std::find_if(res.headers.begin(), res.headers.end(),
                                        [](const mycppwebfw::http::Header& h)
                                        { return h.name == "Content-Type"; });
    EXPECT_NE(content_type_it, res.headers.end());
    EXPECT_EQ(content_type_it->value, "text/html");
    EXPECT_NE(res.content.find("<h1>500</h1>"), std::string::npos);
    EXPECT_NE(res.content.find("<p>Test error</p>"), std::string::npos);
}

TEST(ErrorHandlerTest, ReturnsJsonError)
{
    auto error_handler = mycppwebfw::middleware::create_error_handler();
    mycppwebfw::http::Request req;
    req.headers.push_back({"Accept", "application/json"});
    mycppwebfw::http::Response res;
    auto next = []() { throw std::runtime_error("Test error"); };

    error_handler.handler(req, res, next);

    EXPECT_EQ(res.status,
              mycppwebfw::http::Response::StatusCode::internal_server_error);
    auto content_type_it = std::find_if(res.headers.begin(), res.headers.end(),
                                        [](const mycppwebfw::http::Header& h)
                                        { return h.name == "Content-Type"; });
    EXPECT_NE(content_type_it, res.headers.end());
    EXPECT_EQ(content_type_it->value, "application/json");
    EXPECT_EQ(res.content, "{\"status\": 500, \"error\": \"Test error\"}");
}

TEST(ErrorHandlerTest, ReturnsTextError)
{
    auto error_handler = mycppwebfw::middleware::create_error_handler();
    mycppwebfw::http::Request req;
    // No Accept header, should default to text
    mycppwebfw::http::Response res;
    auto next = []() { throw std::runtime_error("Test error"); };

    error_handler.handler(req, res, next);

    EXPECT_EQ(res.status,
              mycppwebfw::http::Response::StatusCode::internal_server_error);
    auto content_type_it = std::find_if(res.headers.begin(), res.headers.end(),
                                        [](const mycppwebfw::http::Header& h)
                                        { return h.name == "Content-Type"; });
    EXPECT_NE(content_type_it, res.headers.end());
    EXPECT_EQ(content_type_it->value, "text/plain");
    EXPECT_EQ(res.content, "500 Test error");
}

TEST(ErrorHandlerTest, HandlesUnknownException)
{
    auto error_handler = mycppwebfw::middleware::create_error_handler();
    mycppwebfw::http::Request req;
    mycppwebfw::http::Response res;
    auto next = []()
    {
        throw 123;  // Throw something that isn't a std::exception
    };

    error_handler.handler(req, res, next);

    EXPECT_EQ(res.status,
              mycppwebfw::http::Response::StatusCode::internal_server_error);
    auto content_type_it = std::find_if(res.headers.begin(), res.headers.end(),
                                        [](const mycppwebfw::http::Header& h)
                                        { return h.name == "Content-Type"; });
    EXPECT_NE(content_type_it, res.headers.end());
    // Should default to text/plain
    EXPECT_EQ(content_type_it->value, "text/plain");
    EXPECT_EQ(res.content, "500 Internal Server Error");
}
