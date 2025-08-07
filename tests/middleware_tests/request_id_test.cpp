#include "gtest/gtest.h"
#include "mycppwebfw/middleware/request_id.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include <string>
#include <thread>

using namespace mycppwebfw;

namespace mycppwebfw {
namespace middleware {
// This is to access the thread_local variable from the test.
extern thread_local std::string current_request_id;
}
}

TEST(RequestIdMiddlewareTest, GeneratesNewId) {
    auto middleware = middleware::create_request_id_middleware();
    http::Request req;
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    middleware.handler(req, res, next);

    ASSERT_TRUE(next_called);
    std::string request_id;
    for (const auto& header : res.headers) {
        if (header.name == "X-Request-ID") {
            request_id = header.value;
            break;
        }
    }
    ASSERT_FALSE(request_id.empty());
}

TEST(RequestIdMiddlewareTest, ForwardsExistingId) {
    auto middleware = middleware::create_request_id_middleware();
    http::Request req;
    req.headers.push_back({"X-Request-ID", "test-id-123"});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    middleware.handler(req, res, next);

    ASSERT_TRUE(next_called);
    std::string request_id;
    for (const auto& header : res.headers) {
        if (header.name == "X-Request-ID") {
            request_id = header.value;
            break;
        }
    }
    ASSERT_EQ(request_id, "test-id-123");
}

// A simple test to check if the thread_local variable is working.
TEST(RequestIdMiddlewareTest, ThreadLocalId) {
    auto middleware = middleware::create_request_id_middleware();
    http::Request req;
    http::Response res;
    std::string request_id_in_thread;

    auto next = [&]() {
        request_id_in_thread = middleware::current_request_id;
    };

    middleware.handler(req, res, next);

    std::string response_header_id;
    for (const auto& header : res.headers) {
        if (header.name == "X-Request-ID") {
            response_header_id = header.value;
            break;
        }
    }

    ASSERT_FALSE(response_header_id.empty());
    ASSERT_EQ(request_id_in_thread, response_header_id);
}
