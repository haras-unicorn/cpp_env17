#ifndef ENV_BENCH_HPP
#define ENV_BENCH_HPP


#include <env/env.hpp>
#include <env/macros.hpp>

using namespace env::syntax;


#include <benchmark/benchmark.h>


int main(int argc, char** argv)
{
  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
}


#endif // ENV_BENCH_HPP
