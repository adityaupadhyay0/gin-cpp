#include "gtest/gtest.h"
#include "mycppwebfw/middleware/auth.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/utils/api_key_registry.h"
#include "mycppwebfw/utils/jwt_parser.h"
#include <string>
#include <memory>

using namespace mycppwebfw;

TEST(AuthMiddlewareTest, NoAuthHeader) {
    auto api_key_registry = std::make_shared<utils::ApiKeyRegistry>();
    auto jwt_parser = std::make_shared<utils::JwtParser>("secret");
    auto auth = middleware::create_auth_middleware(api_key_registry, jwt_parser);
    http::Request req;
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    auth.handler(req, res, next);

    ASSERT_FALSE(next_called);
    ASSERT_EQ(res.status, http::Response::StatusCode::unauthorized);
}

TEST(AuthMiddlewareTest, InvalidScheme) {
    auto api_key_registry = std::make_shared<utils::ApiKeyRegistry>();
    auto jwt_parser = std::make_shared<utils::JwtParser>("secret");
    auto auth = middleware::create_auth_middleware(api_key_registry, jwt_parser);
    http::Request req;
    req.headers.push_back({"Authorization", "InvalidScheme key"});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    auth.handler(req, res, next);

    ASSERT_FALSE(next_called);
    ASSERT_EQ(res.status, http::Response::StatusCode::unauthorized);
}

TEST(AuthMiddlewareTest, ValidApiKey) {
    auto api_key_registry = std::make_shared<utils::ApiKeyRegistry>();
    api_key_registry->add_key("test-key", "test-user");
    auto jwt_parser = std::make_shared<utils::JwtParser>("secret");
    auto auth = middleware::create_auth_middleware(api_key_registry, jwt_parser);
    http::Request req;
    req.headers.push_back({"Authorization", "ApiKey test-key"});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    auth.handler(req, res, next);

    ASSERT_TRUE(next_called);
}

TEST(AuthMiddlewareTest, InvalidApiKey) {
    auto api_key_registry = std::make_shared<utils::ApiKeyRegistry>();
    api_key_registry->add_key("test-key", "test-user");
    auto jwt_parser = std::make_shared<utils::JwtParser>("secret");
    auto auth = middleware::create_auth_middleware(api_key_registry, jwt_parser);
    http::Request req;
    req.headers.push_back({"Authorization", "ApiKey invalid-key"});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    auth.handler(req, res, next);

    ASSERT_FALSE(next_called);
    ASSERT_EQ(res.status, http::Response::StatusCode::unauthorized);
}

TEST(AuthMiddlewareTest, ValidJwt) {
    auto api_key_registry = std::make_shared<utils::ApiKeyRegistry>();
    auto jwt_parser = std::make_shared<utils::JwtParser>("secret");
    auto auth = middleware::create_auth_middleware(api_key_registry, jwt_parser);

    std::string token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJhdXRoMCJ9.AbIJTDMFc7yUa5MhvcP03nJPyCPzZtQcGEp-zWfOkEE";

    http::Request req;
    req.headers.push_back({"Authorization", "Bearer " + token});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    auth.handler(req, res, next);

    ASSERT_TRUE(next_called);
}

TEST(AuthMiddlewareTest, InvalidJwt) {
    auto api_key_registry = std::make_shared<utils::ApiKeyRegistry>();
    auto jwt_parser = std::make_shared<utils::JwtParser>("secret");
    auto auth = middleware::create_auth_middleware(api_key_registry, jwt_parser);
    http::Request req;
    req.headers.push_back({"Authorization", "Bearer invalid-token"});
    http::Response res;
    bool next_called = false;
    auto next = [&]() { next_called = true; };

    auth.handler(req, res, next);

    ASSERT_FALSE(next_called);
    ASSERT_EQ(res.status, http::Response::StatusCode::unauthorized);
}
