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
