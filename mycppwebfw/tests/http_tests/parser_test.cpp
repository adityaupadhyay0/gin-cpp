#include "gtest/gtest.h"
#include "mycppwebfw/http/parser.h"
#include "mycppwebfw/http/request.h"

TEST(ParserTest, GetRequest) {
    mycppwebfw::http::Request req;
    mycppwebfw::http::Parser parser;
    std::string request_str = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
    auto result = parser.parse(req, request_str.c_str(), request_str.c_str() + request_str.length());

    EXPECT_EQ(result, mycppwebfw::http::Parser::ParseResult::GOOD);
    EXPECT_EQ(req.method, "GET");
    EXPECT_EQ(req.uri, "/index.html");
    EXPECT_EQ(req.http_version_major, 1);
    EXPECT_EQ(req.http_version_minor, 1);
    EXPECT_EQ(req.headers.size(), 1);
    EXPECT_EQ(req.headers[0].name, "Host");
    EXPECT_EQ(req.headers[0].value, "www.example.com");
}

TEST(ParserTest, PostRequest) {
    mycppwebfw::http::Request req;
    mycppwebfw::http::Parser parser;
    std::string request_str = "POST /api/users HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: 27\r\n\r\n{\"name\": \"John Doe\"}";
    auto result = parser.parse(req, request_str.c_str(), request_str.c_str() + request_str.length());

    EXPECT_EQ(result, mycppwebfw::http::Parser::ParseResult::GOOD);
    EXPECT_EQ(req.method, "POST");
    EXPECT_EQ(req.uri, "/api/users");
    EXPECT_EQ(req.http_version_major, 1);
    EXPECT_EQ(req.http_version_minor, 1);
    EXPECT_EQ(req.headers.size(), 2);
    EXPECT_EQ(req.headers[0].name, "Content-Type");
    EXPECT_EQ(req.headers[0].value, "application/json");
    EXPECT_EQ(req.headers[1].name, "Content-Length");
    EXPECT_EQ(req.headers[1].value, "27");
    // NOTE: The current parser does not handle the body of the request.
    // This will be added in a future iteration.
}

TEST(ParserTest, IncompleteRequest) {
    mycppwebfw::http::Request req;
    mycppwebfw::http::Parser parser;
    std::string request_str = "GET /index.html HTTP/1.1\r\n";
    auto result = parser.parse(req, request_str.c_str(), request_str.c_str() + request_str.length());

    EXPECT_EQ(result, mycppwebfw::http::Parser::ParseResult::INDETERMINATE);
}

TEST(ParserTest, BadMethod) {
    mycppwebfw::http::Request req;
    mycppwebfw::http::Parser parser;
    std::string request_str = "123 /index.html HTTP/1.1\r\n\r\n";
    auto result = parser.parse(req, request_str.c_str(), request_str.c_str() + request_str.length());

    EXPECT_EQ(result, mycppwebfw::http::Parser::ParseResult::BAD);
}

TEST(ParserTest, BadURI) {
    mycppwebfw::http::Request req;
    mycppwebfw::http::Parser parser;
    std::string request_str = "GET \t HTTP/1.1\r\n\r\n";
    auto result = parser.parse(req, request_str.c_str(), request_str.c_str() + request_str.length());

    EXPECT_EQ(result, mycppwebfw::http::Parser::ParseResult::BAD);
}

TEST(ParserTest, BadVersion) {
    mycppwebfw::http::Request req;
    mycppwebfw::http::Parser parser;
    std::string request_str = "GET /index.html FTP/1.1\r\n\r\n";
    auto result = parser.parse(req, request_str.c_str(), request_str.c_str() + request_str.length());

    EXPECT_EQ(result, mycppwebfw::http::Parser::ParseResult::BAD);
}
