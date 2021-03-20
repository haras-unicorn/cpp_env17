env_add_dep(bench)

set(BENCHMARK_ENABLE_GTEST_TESTS FALSE CACHE BOOL "Turn off tests for benchmark.")

env_target_link(bench_dep INTERFACE CONAN_PKG::benchmark)
