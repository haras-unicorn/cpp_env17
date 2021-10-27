#include <env/bench.hpp>


void bench(benchmark::State& state)
{
  for ([[maybe_unused]] auto current : state)
  {
    std::vector<int> a{100};
    benchmark::ClobberMemory();
  }
}

BENCHMARK(bench)->Range(1, 10);
