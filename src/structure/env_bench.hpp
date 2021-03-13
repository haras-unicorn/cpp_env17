#ifndef ENV_BENCH_HPP
#define ENV_BENCH_HPP

// benchmark entry point

#include <benchmark/benchmark.h>

// perf macros

// benchmark macros

#if defined(ENV_BENCHMARK)
#define ENV_BENCH(...) BENCHMARK(__VA_ARGS__)
#else // defined(ENV_BENCHMARK)
#define ENV_BENCH(...)
#endif // defined(ENV_BENCHMARK)

#endif // ENV_BENCH_HPP
