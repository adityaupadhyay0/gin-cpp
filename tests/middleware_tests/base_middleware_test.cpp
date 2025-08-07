#include "gtest/gtest.h"
#include "mycppwebfw/middleware/base_middleware.h"
#include "mycppwebfw/context/context.h"
#include "mycppwebfw/utils/middleware_utils.h"
#include <string>
#include <memory>

using namespace mycppwebfw;

class TestMiddleware : public middleware::IMiddleware {
public:
    TestMiddleware(std::string& log) : log_(log) {}

    void operator()(Context&, middleware::Next next) override {
        log_ += "start;";
        next();
        log_ += "end;";
    }

private:
    std::string& log_;
};

TEST(BaseMiddlewareTest, HookChaining) {
    std::string log;
    auto middleware1 = std::make_shared<TestMiddleware>(log);
    auto middleware2 = std::make_shared<TestMiddleware>(log);

    http::Request req;
    http::Response res;
    Context ctx(req, res);

    utils::MiddlewareChainBuilder()
        .add(middleware1)
        .add(middleware2)
        .run(ctx);

    ASSERT_EQ(log, "start;start;end;end;");
}

TEST(BaseMiddlewareTest, ContextPassing) {
    class ContextMiddleware : public middleware::IMiddleware {
    public:
        void operator()(Context& ctx, middleware::Next next) override {
            ctx.set("test_data", std::string("hello"));
            next();
        }
    };

    auto middleware = std::make_shared<ContextMiddleware>();

    http::Request req;
    http::Response res;
    Context ctx(req, res);

    utils::MiddlewareChainBuilder()
        .add(middleware)
        .run(ctx);

    auto* test_data = ctx.get<std::string>("test_data");
    ASSERT_NE(test_data, nullptr);
    ASSERT_EQ(*test_data, "hello");
}

TEST(BaseMiddlewareTest, ErrorForwarding) {
    class ErrorMiddleware : public middleware::IMiddleware {
    public:
        void operator()(Context&, middleware::Next) override {
            throw std::runtime_error("test error");
        }
    };

    auto middleware = std::make_shared<ErrorMiddleware>();

    http::Request req;
    http::Response res;
    Context ctx(req, res);

    ASSERT_THROW(utils::MiddlewareChainBuilder().add(middleware).run(ctx), std::runtime_error);
}
