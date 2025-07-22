#include "gtest/gtest.h"
#include "mycppwebfw/routing/router.h"

void dummy_handler(mycppwebfw::http::Request&, mycppwebfw::http::Response&) {}

TEST(RouterTest, StaticRoute) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler = router.match_route("GET", "/hello", params);
    ASSERT_NE(handler, nullptr);
}

TEST(RouterTest, ParameterizedRoute) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler = router.match_route("GET", "/users/123", params);
    ASSERT_NE(handler, nullptr);
    ASSERT_EQ(params["id"], "123");
}

TEST(RouterTest, WildcardRoute) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/static/*path", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler = router.match_route("GET", "/static/css/style.css", params);
    ASSERT_NE(handler, nullptr);
    ASSERT_EQ(params["path"], "css/style.css");
}

TEST(RouterTest, RouteNotFound) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler = router.match_route("GET", "/world", params);
    ASSERT_EQ(handler, nullptr);
}

TEST(RouterTest, MethodNotFound) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler = router.match_route("POST", "/hello", params);
    ASSERT_EQ(handler, nullptr);
}

TEST(RouterTest, OverlappingRoutes) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/new", dummy_handler);
    router.add_route("GET", "/users/:id", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler1 = router.match_route("GET", "/users/new", params);
    ASSERT_NE(handler1, nullptr);
    auto handler2 = router.match_route("GET", "/users/123", params);
    ASSERT_NE(handler2, nullptr);
    ASSERT_EQ(params["id"], "123");
}
