#include "gtest/gtest.h"
#include "mycppwebfw/routing/router.h"
#include "mycppwebfw/routing/parameter_parser.h"

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

TEST(RouterTest, OptionalParameter) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id?", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler1 = router.match_route("GET", "/users/123", params);
    ASSERT_NE(handler1, nullptr);
    ASSERT_EQ(params["id"], "123");
    params.clear();
    auto handler2 = router.match_route("GET", "/users", params);
    ASSERT_NE(handler2, nullptr);
    ASSERT_EQ(params.find("id"), params.end());
}

TEST(RouterTest, DefaultParameter) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/lang/:locale=en", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler1 = router.match_route("GET", "/lang/fr", params);
    ASSERT_NE(handler1, nullptr);
    ASSERT_EQ(params["locale"], "fr");
    params.clear();
    auto handler2 = router.match_route("GET", "/lang", params);
    ASSERT_NE(handler2, nullptr);
    ASSERT_EQ(params["locale"], "en");
}

TEST(RouterTest, WildcardPrecedence) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/user/specific", dummy_handler);
    router.add_route("GET", "/user/*path", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    auto handler = router.match_route("GET", "/user/specific", params);
    ASSERT_NE(handler, nullptr);
    ASSERT_EQ(params.size(), 0);
}

TEST(RouterTest, ParameterTypeConversion) {
    std::string id_str = "123";
    int id_int = mycppwebfw::routing::ParameterParser::parse<int>(id_str);
    ASSERT_EQ(id_int, 123);

    std::string pi_str = "3.14";
    float pi_float = mycppwebfw::routing::ParameterParser::parse<float>(pi_str);
    ASSERT_FLOAT_EQ(pi_float, 3.14f);
}

TEST(RouterTest, ParameterValidation) {
    std::string valid_email = "test@example.com";
    std::string invalid_email = "not-an-email";
    std::string email_pattern = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
    ASSERT_TRUE(mycppwebfw::routing::ParameterParser::validate(valid_email, email_pattern));
    ASSERT_FALSE(mycppwebfw::routing::ParameterParser::validate(invalid_email, email_pattern));
}
