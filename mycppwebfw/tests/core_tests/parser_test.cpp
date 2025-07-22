#include <gtest/gtest.h>
#include "mycppwebfw/http/parser.h"
#include "mycppwebfw/http/request.h"

using namespace mycppwebfw;

TEST(HttpParserTest, ValidRequest) {
    http::Request req;
    std::string raw = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    http::Parser parser;
    auto result = parser.parse(req, raw.data(), raw.data() + raw.size());
    ASSERT_EQ(result, http::Parser::ParseResult::GOOD);
    ASSERT_EQ(req.method, "GET");
    ASSERT_EQ(req.uri, "/");
    ASSERT_EQ(req.http_version_major, 1);
    ASSERT_EQ(req.http_version_minor, 1);
    auto host_hdr = std::find_if(req.headers.begin(), req.headers.end(),
        [](const http::Header& h) { return h.name == "Host"; });
    ASSERT_NE(host_hdr, req.headers.end());
    ASSERT_EQ(host_hdr->value, "localhost");
}

TEST(HttpParserTest, InvalidRequest) {
    http::Request req;
    std::string raw = "BADREQUEST\r\n\r\n";
    http::Parser parser;
    auto result = parser.parse(req, raw.data(), raw.data() + raw.size());
    ASSERT_EQ(result, http::Parser::ParseResult::BAD);
}
