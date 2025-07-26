#include "gtest/gtest.h"
#include "mycppwebfw/middleware/middleware_chain.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/routing/router.h"

TEST(MiddlewareChainTest, ChainExecutionInOrder)
{
    std::vector<mycppwebfw::middleware::Middleware> middlewares;
    std::vector<int> execution_order;

    middlewares.push_back({[&](auto&, auto&, auto next) {
        execution_order.push_back(1);
        next();
    }});

    middlewares.push_back({[&](auto&, auto&, auto next) {
        execution_order.push_back(2);
        next();
    }});

    mycppwebfw::HttpHandler final_handler = [&](auto&, auto&) {
        execution_order.push_back(3);
    };

    mycppwebfw::middleware::MiddlewareChain chain(middlewares, final_handler);
    mycppwebfw::http::Request req;
    mycppwebfw::http::Response res;
    chain.next(req, res);

    ASSERT_EQ(execution_order.size(), 3);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 2);
    EXPECT_EQ(execution_order[2], 3);
}

TEST(MiddlewareChainTest, GlobalMiddlewareRunsOnAllRoutes)
{
    mycppwebfw::routing::Router router;
    std::vector<int> execution_order;

    router.use({[&](auto&, auto&, auto next) {
        execution_order.push_back(1);
        next();
    }});

    router.add_route("GET", "/test", [&](auto&, auto&) {
        execution_order.push_back(2);
    });

    mycppwebfw::http::Request req;
    req.method = "GET";
    req.uri = "/test";
    std::unordered_map<std::string, std::string> params;
    auto match = router.match_route(req, params);

    ASSERT_NE(match.handler, nullptr);
    mycppwebfw::http::Response res;
    match.handler(req, res);

    ASSERT_EQ(execution_order.size(), 2);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 2);
}

TEST(MiddlewareChainTest, RouteSpecificMiddleware)
{
    mycppwebfw::routing::Router router;
    std::vector<int> execution_order;

    router.add_route(
        "GET", "/test",
        [&](auto&, auto&) {
            execution_order.push_back(2);
        },
        {{{[&](auto&, auto&, auto next) {
            execution_order.push_back(1);
            next();
        }}}});

    mycppwebfw::http::Request req;
    req.method = "GET";
    req.uri = "/test";
    std::unordered_map<std::string, std::string> params;
    auto match = router.match_route(req, params);

    ASSERT_NE(match.handler, nullptr);
    mycppwebfw::http::Response res;
    match.handler(req, res);

    ASSERT_EQ(execution_order.size(), 2);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 2);
}

TEST(MiddlewareChainTest, MiddlewareEarlyExit)
{
    std::vector<mycppwebfw::middleware::Middleware> middlewares;
    std::vector<int> execution_order;

    middlewares.push_back({[&](auto&, auto&, auto) {
        execution_order.push_back(1);
        // Don't call next()
    }});

    middlewares.push_back({[&](auto&, auto&, auto next) {
        execution_order.push_back(2);
        next();
    }});

    mycppwebfw::HttpHandler final_handler = [&](auto&, auto&) {
        execution_order.push_back(3);
    };

    mycppwebfw::middleware::MiddlewareChain chain(middlewares, final_handler);
    mycppwebfw::http::Request req;
    mycppwebfw::http::Response res;
    chain.next(req, res);

    ASSERT_EQ(execution_order.size(), 1);
    EXPECT_EQ(execution_order[0], 1);
}

TEST(MiddlewareChainTest, NextNotCalled)
{
    std::vector<mycppwebfw::middleware::Middleware> middlewares;
    std::vector<int> execution_order;

    middlewares.push_back({[&](auto&, auto&, auto) {
        execution_order.push_back(1);
        // Don't call next()
    }});

    mycppwebfw::HttpHandler final_handler = [&](auto&, auto&) {
        execution_order.push_back(2);
    };

    mycppwebfw::middleware::MiddlewareChain chain(middlewares, final_handler);
    mycppwebfw::http::Request req;
    mycppwebfw::http::Response res;
    chain.next(req, res);

    ASSERT_EQ(execution_order.size(), 1);
    EXPECT_EQ(execution_order[0], 1);
}

TEST(MiddlewareChainTest, MiddlewareOrdering)
{
    std::vector<mycppwebfw::middleware::Middleware> middlewares;
    std::vector<int> execution_order;

    middlewares.push_back({{[&](auto&, auto&, auto next) {
        execution_order.push_back(2);
        next();
    }}, 2});

    middlewares.push_back({{[&](auto&, auto&, auto next) {
        execution_order.push_back(1);
        next();
    }}, 1});

    mycppwebfw::HttpHandler final_handler = [&](auto&, auto&) {
        execution_order.push_back(3);
    };

    mycppwebfw::routing::Router router;
    router.add_route(
        "GET", "/test", final_handler, middlewares);

    mycppwebfw::http::Request req;
    req.method = "GET";
    req.uri = "/test";
    std::unordered_map<std::string, std::string> params;
    auto match = router.match_route(req, params);

    ASSERT_NE(match.handler, nullptr);
    mycppwebfw::http::Response res;
    match.handler(req, res);

    ASSERT_EQ(execution_order.size(), 3);
    EXPECT_EQ(execution_order[0], 1);
    EXPECT_EQ(execution_order[1], 2);
    EXPECT_EQ(execution_order[2], 3);
}
