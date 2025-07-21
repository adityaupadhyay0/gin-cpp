# Core Module: Signal Handling & Graceful Shutdown

# Testing & Benchmarking

## Unit Testing
- All core components (ThreadPool, Server, BufferPool, Connection, Middleware) have GoogleTest coverage in `tests/core_tests/`
- Signal handling and resource cleanup are tested

## Integration Testing
- Simulate multiple clients, concurrent requests, shutdown/reload
- Validate resource cleanup under load

## Performance Benchmarks
- See `benchmarks/core_bench/server_bench.cpp` for server throughput

## Example Test
```cpp
TEST(ThreadPoolTest, EnqueueAndExecuteTask) {
    // ...
}
```

## See Also
- `docs/api_reference.md` for API details
- `docs/getting_started.md` for quickstart

## Signal Handling
- Integrated Asio-based signal handling for SIGINT/SIGTERM (graceful shutdown) and SIGHUP (config reload).
- Server automatically closes connections, releases buffers, and stops thread pool on shutdown signal.
- Configuration reload is supported via SIGHUP (on Unix) or custom signal.

## Resource Cleanup
- All connections, buffers, and threads are properly closed on shutdown.
- Buffer pool and thread pool are released and joined.

## Example Usage
```cpp
mycppwebfw::core::Server server("127.0.0.1", "8080");
server.run(); // Handles signals and cleans up resources automatically
```

## Testing
- Unit tests in `core_tests/server_test.cpp` simulate signals and verify cleanup.

## See Also
- `docs/howto/deploy_with_docker.md` for deployment and signal handling in containers.
