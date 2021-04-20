#define ENV_COMPILER_MESSAGES

#include <env/env.hpp>

#include <benchmark/benchmark.h>


void bench(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        env::immut::vector<int> a{100};
        std::fill(a.begin(), a.end(), 1);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(bench);


BENCHMARK_MAIN();
