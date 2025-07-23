#include "benchmark/benchmark.h"
#include "mycppwebfw/routing/router.h"

void dummy_handler(mycppwebfw::http::Request&, mycppwebfw::http::Response&) {}

static void BM_Router_Static(benchmark::State& state) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    for (auto _ : state) {
        std::unordered_map<std::string, std::string> params;
        router.match_route("GET", "/hello", params);
    }
}
BENCHMARK(BM_Router_Static);

static void BM_Router_Parameterized(benchmark::State& state) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id", dummy_handler);
    for (auto _ : state) {
        std::unordered_map<std::string, std::string> params;
        router.match_route("GET", "/users/123", params);
    }
}
BENCHMARK(BM_Router_Parameterized);

static void BM_Router_Wildcard(benchmark::State& state) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/static/*path", dummy_handler);
    for (auto _ : state) {
        std::unordered_map<std::string, std::string> params;
        router.match_route("GET", "/static/css/style.css", params);
    }
}
BENCHMARK(BM_Router_Wildcard);

static void BM_OptionalParameterRoute(benchmark::State& state) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id?", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    for (auto _ : state) {
        router.match_route("GET", "/users/123", params);
    }
}
BENCHMARK(BM_OptionalParameterRoute);

static void BM_DefaultParameterRoute(benchmark::State& state) {
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/lang/:locale=en", dummy_handler);
    std::unordered_map<std::string, std::string> params;
    for (auto _ : state) {
        router.match_route("GET", "/lang", params);
    }
}
BENCHMARK(BM_DefaultParameterRoute);

BENCHMARK_MAIN();
