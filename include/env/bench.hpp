#ifndef ENV_BENCH_HPP
#define ENV_BENCH_HPP


#include <env/env.hpp>
#include <env/macros.hpp>

using namespace env::syntax;


#include <benchmark/benchmark.h>

// ignore GoogleBenchmark static storage warning
#define ENV_BENCH_CASE(_bench) BENCHMARK(_bench) // NOLINT(cert-err58-cpp)


BENCHMARK_MAIN();


#endif // ENV_BENCH_HPP
