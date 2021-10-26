#include <env/bench.hpp>


void bench(benchmark::State& state)
{
    for ([[maybe_unused]] auto current : state)
    {
        std::vector<int> a{100};
        benchmark::ClobberMemory();
    }
}

ENV_BENCH_CASE(bench)->Range(1, 10);
