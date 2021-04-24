#include <env/bench.hpp>


void bench(benchmark::State& state)
{
    for ([[maybe_unused]] auto current : state)
    {
        env::immut::vector<int> a{100};
        env::trans::noop(a);
        benchmark::ClobberMemory();
    }
}

ENV_BENCH(bench)->Range(1, 10);
