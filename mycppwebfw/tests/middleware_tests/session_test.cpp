#include "mycppwebfw/middleware/session.h"
#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/utils/session_store.h"

using namespace mycppwebfw;

TEST(SessionMiddlewareTest, NewSession)
{
    auto store = std::make_shared<utils::InMemorySessionStore>();
    auto session_middleware = middleware::create_session_middleware(store);
    http::Request req;
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    session_middleware.handler(req, res, next);

    ASSERT_TRUE(next_called);
    ASSERT_EQ(res.headers.size(), 1);
    ASSERT_EQ(res.headers[0].name, "Set-Cookie");
    ASSERT_NE(res.headers[0].value.find("session_id="), std::string::npos);
}

TEST(SessionMiddlewareTest, ExistingSession)
{
    auto store = std::make_shared<utils::InMemorySessionStore>();
    auto session_middleware = middleware::create_session_middleware(store);
    http::Request req;
    req.headers.push_back({"Cookie", "session_id=test-session-id"});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    session_middleware.handler(req, res, next);

    ASSERT_TRUE(next_called);
    ASSERT_EQ(res.headers.size(), 0);
    bool session_id_found = false;
    for (const auto& header : req.headers)
    {
        if (header.name == "X-Session-ID" && header.value == "test-session-id")
        {
            session_id_found = true;
            break;
        }
    }
    ASSERT_TRUE(session_id_found);
}
