#ifndef ENV_BENCH_HPP
#define ENV_BENCH_HPP


// NOTE: test on GCC
// there is no way to stop Clang from optimizing away some things and MSVC just don't...


// benchmark entry point

#include <benchmark/benchmark.h>


// perf macros


// benchmark macros

#if defined(ENV_BENCHMARK)
#define ENV_BENCH(_name) static void _name(ENV_GBENCH::State& state)
#define ENV_TIME(_name, ...) BENCHMARK(_name) __VA_ARGS__ ;
#else // defined(ENV_BENCHMARK)
#define ENV_BENCH(_name) template<typename> [[maybe_unused]] static void _name(ENV_GBENCH::State& state)
#define ENV_TIME(_name, ...)
#endif // defined(ENV_BENCHMARK)

#define ENV_BLOOP for ([[maybe_unused]] auto _ : state)


#endif // ENV_BENCH_HPP
