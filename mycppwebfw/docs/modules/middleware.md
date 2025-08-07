# Documentation for middleware module

This document describes the middleware system in `mycppwebfw`.

## Concepts

### IMiddleware

The `IMiddleware` interface is an abstract base class that all middleware must inherit from. It has a single pure virtual function, `operator()`, which takes a `Context` object and a `Next` function.

### Context

The `Context` object holds request-specific data and is passed to each middleware. It contains the `http::Request` and `http::Response` objects, as well as a generic data store that can be used to pass data between middleware.

### MiddlewareChainBuilder

The `MiddlewareChainBuilder` is a utility class that is used to build and run a chain of middleware.

## Creating Middleware

To create a new middleware, you need to create a class that inherits from `IMiddleware` and implements the `operator()` method.

Here is an example of a simple middleware that logs the request method and path:

```cpp
#include "mycppwebfw/middleware/base_middleware.h"
#include "mycppwebfw/context/context.h"
#include <iostream>

class LoggerMiddleware : public mycppwebfw::middleware::IMiddleware {
public:
    void operator()(mycppwebfw::Context& ctx, mycppwebfw::middleware::Next next) override {
        std::cout << "Request: " << ctx.req().get_method() << " " << ctx.req().get_path() << std::endl;
        next();
    }
};
```

## Using Middleware

To use a middleware, you need to add it to a `MiddlewareChainBuilder` and then run the chain.

```cpp
#include "mycppwebfw/utils/middleware_utils.h"
#include "mycppwebfw/context/context.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include <memory>

// ...

auto logger = std::make_shared<LoggerMiddleware>();

mycppwebfw::http::Request req;
mycppwebfw::http::Response res;
mycppwebfw::Context ctx(req, res);

mycppwebfw::utils::MiddlewareChainBuilder()
    .add(logger)
    .run(ctx);
```

## Existing Middleware

### Request ID Middleware

The `RequestId` middleware generates a unique request ID for each incoming request. This ID is then added to the response headers as `X-Request-ID`.

If the incoming request already has an `X-Request-ID` header, the middleware will use that ID instead of generating a new one.

This middleware is useful for tracing requests as they travel through different services in a microservice architecture.

### Authentication Middleware

The `Auth` middleware provides a flexible authentication framework for the application. It supports API Key and JWT authentication schemes.

### Session Middleware

The `Session` middleware provides session management for the application. It uses a cookie to store a session ID, and a session store to store session data.
