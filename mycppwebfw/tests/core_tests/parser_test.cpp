#include <gtest/gtest.h>
#include "mycppwebfw/core/http/parser.h"
#include "mycppwebfw/core/http/request.h"

TEST(HttpParserTest, ValidRequest) {
    http::Request req;
    std::string raw = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    http::Parser parser;
    auto result = parser.parse(req, raw.data(), raw.data() + raw.size());
    ASSERT_EQ(result, http::Parser::ParseResult::GOOD);
    ASSERT_EQ(req.method, "GET");
    ASSERT_EQ(req.uri, "/");
    ASSERT_EQ(req.http_version, "1.1");
    ASSERT_EQ(req.headers["Host"], "localhost");
}

TEST(HttpParserTest, InvalidRequest) {
    http::Request req;
    std::string raw = "BADREQUEST\r\n\r\n";
    http::Parser parser;
    auto result = parser.parse(req, raw.data(), raw.data() + raw.size());
    ASSERT_EQ(result, http::Parser::ParseResult::BAD);
}
