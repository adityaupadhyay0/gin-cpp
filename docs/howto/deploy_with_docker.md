# Deploying with Docker: Signal Handling & Graceful Shutdown

## Signal Handling in Containers
- The server uses Asio to handle SIGINT/SIGTERM for graceful shutdown.
- When running in Docker, signals sent via `docker stop` or `docker kill` (with SIGTERM/SIGINT) will trigger cleanup.

## Configuration Reload
- SIGHUP or custom signals can be sent to reload configuration at runtime (Unix only).
- Example: `docker kill --signal=SIGHUP <container>`

## Resource Cleanup
- On shutdown, all connections, buffers, and threads are closed and released.

## Example Dockerfile Entrypoint
```Dockerfile
CMD ["/app/mycppwebfw_server"]
```

## Best Practices
- Use `docker stop` for graceful shutdown.
- For config reload, send SIGHUP if supported by your base image.

## References
- See `docs/modules/core.md` for more details on signal handling and cleanup.
