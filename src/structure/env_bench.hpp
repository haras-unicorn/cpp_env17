#ifndef ENV_BENCH_HPP
#define ENV_BENCH_HPP

// benchmark entry point

#include <benchmark/benchmark.h>

// perf macros

// benchmark macros

#if defined(ENV_BENCHMARK)
#define ENV_BENCH(_name)                             \
        static void _name(ENV_GBENCH::State& state); \
        BENCHMARK(_name);                            \
        static void _name(ENV_GBENCH::State& state)
#else // defined(ENV_BENCHMARK)
#define ENV_BENCH(_name)
#endif // defined(ENV_BENCHMARK)

#endif // ENV_BENCH_HPP
