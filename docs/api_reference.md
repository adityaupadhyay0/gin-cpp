# API Reference: Gin-like C++ Web Framework

## Core Classes
- Server: Lifecycle, signal handling, config reload
- ThreadPool: Worker management, dynamic resizing
- BufferPool: Zero-copy, memory limits, custom allocator
- Connection: Lifecycle, keep-alive, timeouts, state
- Middleware: Chain, error handling, registration

## Configuration
- ThreadPoolConfig: min/max threads, idle timeout, queue size
- BufferPool: buffer size, pool size, memory limit
- SignalHandler: SIGINT/SIGTERM/SIGHUP support

## Example Usage
```cpp
mycppwebfw::core::Server server("127.0.0.1", "8080");
server.run();
```

## See Also
- `docs/getting_started.md` for quickstart
- `docs/modules/core.md` for internals
