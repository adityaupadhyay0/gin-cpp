# Getting Started: Gin-like C++ Web Framework

## Minimal Example
```cpp
#include "mycppwebfw/core/server.h"
int main() {
    mycppwebfw::core::Server server("127.0.0.1", "8080");
    server.run();
}
```

## Routing & Middleware
- See `docs/modules/core.md` for advanced usage

## Configuration Reload
- Send SIGHUP (Unix) or use custom signal for live config reload

## Testing
- Run tests in `tests/core_tests/` for coverage
