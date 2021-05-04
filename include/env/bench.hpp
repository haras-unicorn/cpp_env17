#ifndef ENV_BENCH_HPP
#define ENV_BENCH_HPP


#include <env/env.hpp>
#include <env/macros.hpp>

using namespace env::syntax;


#include <benchmark/benchmark.h>

// ignore GoogleBenchmark static storage warning
#define ENV_BENCH_CASE(_bench) BENCHMARK(_bench) // NOLINT(cert-err58-cpp)


ENV_GNU_SUPPRESS_PUSH("-Wmisleading-indentation");

BENCHMARK_MAIN();

ENV_GNU_SUPPRESS_POP;


#endif // ENV_BENCH_HPP
