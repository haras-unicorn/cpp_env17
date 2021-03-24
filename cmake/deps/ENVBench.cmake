env_add_dep(bench)

set(BENCHMARK_ENABLE_GTEST_TESTS FALSE CACHE BOOL "Turn off tests for benchmark.")

env_fetch(
        bench
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.5.2
)

env_target_link(bench_dep INTERFACE benchmark)
