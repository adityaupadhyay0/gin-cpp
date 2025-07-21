// Benchmark: HTTP server throughput
#include <chrono>
#include <iostream>
#include "mycppwebfw/core/server.h"

int main() {
    mycppwebfw::core::Server server("127.0.0.1", "8080");
    auto start = std::chrono::high_resolution_clock::now();
    server.run();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Server ran for " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds.\n";
    // Add request throughput measurement here
    return 0;
}
