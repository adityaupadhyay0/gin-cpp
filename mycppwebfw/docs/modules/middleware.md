# Documentation for middleware module

## Request ID Middleware

The `RequestId` middleware generates a unique request ID for each incoming request. This ID is then added to the response headers as `X-Request-ID`.

If the incoming request already has an `X-Request-ID` header, the middleware will use that ID instead of generating a new one.

This middleware is useful for tracing requests as they travel through different services in a microservice architecture.

### Usage

To use the `RequestId` middleware, you can add it to your router like this:

```cpp
#include "mycppwebfw/middleware/request_id.h"

// ...

router.use(mycppwebfw::middleware::create_request_id_middleware());
```

## Authentication Middleware

The `Auth` middleware provides a flexible authentication framework for the application. It supports API Key and JWT authentication schemes.

### Usage

To use the `Auth` middleware, you first need to create an instance of the `ApiKeyRegistry` and the `JwtParser`. Then you can create the middleware and add it to your router.

```cpp
#include "mycppwebfw/middleware/auth.h"
#include "mycppwebfw/utils/api_key_registry.h"
#include "mycppwebfw/utils/jwt_parser.h"
#include <memory>

// ...

auto api_key_registry = std::make_shared<mycppwebfw::utils::ApiKeyRegistry>();
api_key_registry->add_key("my-secret-key", "my-user");

auto jwt_parser = std::make_shared<mycppwebfw::utils::JwtParser>("my-jwt-secret");

router.use(mycppwebfw::middleware::create_auth_middleware(api_key_registry, jwt_parser));
```

The middleware will check for the `Authorization` header in the incoming request. The header should be in one of the following formats:
*   `Authorization: ApiKey <key>`
*   `Authorization: Bearer <token>`

If the header is not present or the key/token is invalid, the middleware will return a `401 Unauthorized` response.

## Session Middleware

The `Session` middleware provides session management for the application. It uses a cookie to store a session ID, and a session store to store session data.

### Usage

To use the `Session` middleware, you first need to create an instance of a `SessionStore`. Then you can create the middleware and add it to your router.

```cpp
#include "mycppwebfw/middleware/session.h"
#include "mycppwebfw/utils/session_store.h"
#include <memory>

// ...

auto session_store = std::make_shared<mycppwebfw::utils::InMemorySessionStore>();

router.use(mycppwebfw::middleware::create_session_middleware(session_store));
```

The middleware will check for a `session_id` cookie in the incoming request. If the cookie is not present, it will generate a new session ID and set it in a `Set-Cookie` header in the response.
