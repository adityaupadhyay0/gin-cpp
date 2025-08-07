#include "mycppwebfw/middleware/request_id.h"
#include <string>
#include "gtest/gtest.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"

using namespace mycppwebfw;

TEST(RequestIdMiddlewareTest, GeneratesNewId)
{
    auto middleware = middleware::create_request_id_middleware();
    http::Request req;
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    middleware.handler(req, res, next);

    ASSERT_TRUE(next_called);
    std::string request_id;
    for (const auto& header : res.headers)
    {
        if (header.name == "X-Request-ID")
        {
            request_id = header.value;
            break;
        }
    }
    ASSERT_FALSE(request_id.empty());
}

TEST(RequestIdMiddlewareTest, ForwardsExistingId)
{
    auto middleware = middleware::create_request_id_middleware();
    http::Request req;
    req.headers.push_back({"X-Request-ID", "test-id-123"});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    middleware.handler(req, res, next);

    ASSERT_TRUE(next_called);
    std::string request_id;
    for (const auto& header : res.headers)
    {
        if (header.name == "X-Request-ID")
        {
            request_id = header.value;
            break;
        }
    }
    ASSERT_EQ(request_id, "test-id-123");
}
