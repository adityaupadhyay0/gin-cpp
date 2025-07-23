#include "mycppwebfw/routing/router.h"
#include "gtest/gtest.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/routing/parameter_parser.h"

void dummy_handler(mycppwebfw::http::Request&, mycppwebfw::http::Response&)
{
}

TEST(RouterTest, StaticRoute)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"GET", "/hello"};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
}

TEST(RouterTest, ParameterizedRoute)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"GET", "/users/123"};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
    ASSERT_EQ(params["id"], "123");
}

TEST(RouterTest, WildcardRoute)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/static/*path", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"GET", "/static/css/style.css"};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
    ASSERT_EQ(params["path"], "css/style.css");
}

TEST(RouterTest, RouteNotFound)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"GET", "/world"};
    auto result = router.match_route(req, params);
    ASSERT_EQ(result.handler, nullptr);
}

TEST(RouterTest, MethodNotFound)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"POST", "/hello"};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
    ASSERT_EQ(result.allowed_methods.size(), 1);
    ASSERT_EQ(result.allowed_methods[0], "GET");
}

TEST(RouterTest, OverlappingRoutes)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/new", dummy_handler);
    router.add_route("GET", "/users/:id", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req1 = {"GET", "/users/new"};
    auto result1 = router.match_route(req1, params);
    ASSERT_NE(result1.handler, nullptr);
    mycppwebfw::http::Request req2 = {"GET", "/users/123"};
    auto result2 = router.match_route(req2, params);
    ASSERT_NE(result2.handler, nullptr);
    ASSERT_EQ(params["id"], "123");
}

TEST(RouterTest, OptionalParameter)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id?", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req1 = {"GET", "/users/123"};
    auto result1 = router.match_route(req1, params);
    ASSERT_NE(result1.handler, nullptr);
    ASSERT_EQ(params["id"], "123");
    params.clear();
    mycppwebfw::http::Request req2 = {"GET", "/users"};
    auto result2 = router.match_route(req2, params);
    ASSERT_NE(result2.handler, nullptr);
    ASSERT_EQ(params.find("id"), params.end());
}

TEST(RouterTest, DefaultParameter)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/lang/:locale=en", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req1 = {"GET", "/lang/fr"};
    auto result1 = router.match_route(req1, params);
    ASSERT_NE(result1.handler, nullptr);
    ASSERT_EQ(params["locale"], "fr");
    params.clear();
    mycppwebfw::http::Request req2 = {"GET", "/lang"};
    auto result2 = router.match_route(req2, params);
    ASSERT_NE(result2.handler, nullptr);
    ASSERT_EQ(params["locale"], "en");
}

TEST(RouterTest, WildcardPrecedence)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/user/specific", dummy_handler);
    router.add_route("GET", "/user/*path", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"GET", "/user/specific"};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
    ASSERT_EQ(params.size(), 0);
}

TEST(RouterTest, ParameterTypeConversion)
{
    std::string id_str = "123";
    int id_int = mycppwebfw::routing::ParameterParser::parse<int>(id_str);
    ASSERT_EQ(id_int, 123);

    std::string pi_str = "3.14";
    float pi_float = mycppwebfw::routing::ParameterParser::parse<float>(pi_str);
    ASSERT_FLOAT_EQ(pi_float, 3.14f);
}

TEST(RouterTest, ParameterValidation)
{
    std::string valid_email = "test@example.com";
    std::string invalid_email = "not-an-email";
    std::string email_pattern =
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
    ASSERT_TRUE(mycppwebfw::routing::ParameterParser::validate(valid_email,
                                                               email_pattern));
    ASSERT_FALSE(mycppwebfw::routing::ParameterParser::validate(invalid_email,
                                                                email_pattern));
}

TEST(RouterTest, MethodOverrideHeader)
{
    mycppwebfw::routing::Router router;
    router.add_route("PUT", "/users/123", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {
        "POST", "/users/123", 1, 1, {{"X-HTTP-Method-Override", "PUT"}}, ""};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
}

TEST(RouterTest, MethodOverrideQuery)
{
    mycppwebfw::routing::Router router;
    router.add_route("PUT", "/users/123", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"POST", "/users/123?_method=PUT"};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
}

TEST(RouterTest, Middleware)
{
    mycppwebfw::routing::Router router;
    bool middleware_called = false;
    mycppwebfw::routing::HttpHandler middleware =
        [&](mycppwebfw::http::Request&, mycppwebfw::http::Response&)
    { middleware_called = true; };
    router.add_route("GET", "/hello", dummy_handler, {middleware});
    std::unordered_map<std::string, std::string> params;
    mycppwebfw::http::Request req = {"GET", "/hello"};
    auto result = router.match_route(req, params);
    ASSERT_NE(result.handler, nullptr);
    ASSERT_EQ(result.middlewares.size(), 1);
    mycppwebfw::http::Response res;
    result.middlewares[0](req, res);
    ASSERT_TRUE(middleware_called);
}
