#include "benchmark/benchmark.h"
#include "mycppwebfw/http/request.h"
#include "mycppwebfw/http/response.h"
#include "mycppwebfw/routing/router.h"

void dummy_handler(mycppwebfw::http::Request&, mycppwebfw::http::Response&)
{
}

static void BM_Router_Static(benchmark::State& state)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    mycppwebfw::http::Request req = {"GET", "/hello"};
    for (auto _ : state)
    {
        std::unordered_map<std::string, std::string> params;
        router.match_route(req, params);
    }
}
BENCHMARK(BM_Router_Static);

static void BM_Router_Parameterized(benchmark::State& state)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id", dummy_handler);
    mycppwebfw::http::Request req = {"GET", "/users/123"};
    for (auto _ : state)
    {
        std::unordered_map<std::string, std::string> params;
        router.match_route(req, params);
    }
}
BENCHMARK(BM_Router_Parameterized);

static void BM_Router_Wildcard(benchmark::State& state)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/static/*path", dummy_handler);
    mycppwebfw::http::Request req = {"GET", "/static/css/style.css"};
    for (auto _ : state)
    {
        std::unordered_map<std::string, std::string> params;
        router.match_route(req, params);
    }
}
BENCHMARK(BM_Router_Wildcard);

static void BM_OptionalParameterRoute(benchmark::State& state)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/users/:id?", dummy_handler);
    mycppwebfw::http::Request req = {"GET", "/users/123"};
    std::unordered_map<std::string, std::string> params;
    for (auto _ : state)
    {
        router.match_route(req, params);
    }
}
BENCHMARK(BM_OptionalParameterRoute);

static void BM_DefaultParameterRoute(benchmark::State& state)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/lang/:locale=en", dummy_handler);
    mycppwebfw::http::Request req = {"GET", "/lang"};
    std::unordered_map<std::string, std::string> params;
    for (auto _ : state)
    {
        router.match_route(req, params);
    }
}
BENCHMARK(BM_DefaultParameterRoute);

static void BM_Router_MultiMethod(benchmark::State& state)
{
    mycppwebfw::routing::Router router;
    router.add_route("GET", "/hello", dummy_handler);
    router.add_route("POST", "/hello", dummy_handler);
    router.add_route("PUT", "/hello", dummy_handler);
    router.add_route("DELETE", "/hello", dummy_handler);
    mycppwebfw::http::Request req = {"DELETE", "/hello"};
    for (auto _ : state)
    {
        std::unordered_map<std::string, std::string> params;
        router.match_route(req, params);
    }
}
BENCHMARK(BM_Router_MultiMethod);

BENCHMARK_MAIN();
