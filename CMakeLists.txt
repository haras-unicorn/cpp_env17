cmake_minimum_required(VERSION 3.17...3.20)

project(env
        DESCRIPTION "Environment for all of my C++ projects."
        VERSION 1.0
        LANGUAGES C CXX)

include(./cmake/EnvFunc.cmake)

env_init_project()


env_log(-!- Fetching env dependencies... -!-)

env_fetch(macros
          GIT_REPOSITORY https://github.com/nemequ/hedley.git
          GIT_TAG v15)

env_fetch(hana
          GIT_REPOSITORY https://github.com/Hrle97/hana.git
          GIT_TAG cmake

          OPTIONS
          "BOOS_HANA_BUILD_TESTS OFF"
          "BOOS_HANA_BUILD_EXAMPLES OFF"
          "BOOS_HANA_BUILD_BENCHMARKS OFF"
          "BOOS_HANA_BUILD_DOCS OFF"
          "BOOS_HANA_BUILD_EXTRAS OFF")

env_fetch(test
          GIT_REPOSITORY https://github.com/google/googletest.git
          GIT_TAG release-1.10.0

          OPTIONS
          "INSTALL_GTEST OFF"
          "gtest_force_shared_crt ON"
          "gtest_build_tests OFF"
          "gtest_build_examples OFF"
          "gmock_build_tests OFF")

env_fetch(benchmark
          GIT_REPOSITORY https://github.com/google/benchmark.git
          GIT_TAG v1.5.2

          OPTIONS
          "BENCHMARK_ENABLE_TESTING OFF"
          "BENCHMARK_ENABLE_INSTALL OFF"
          "BENCHMARK_ENABLE_GTEST_TESTS OFF")

env_fetch(alloc
          GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
          GIT_TAG v2.0.0

          OPTIONS
          "MI_USE_CXX ON"
          "MI_BUILD_SHARED OFF"
          "MI_BUILD_OBJECT OFF"
          "MI_BUILD_TESTS OFF")

env_fetch(json
          GIT_REPOSITORY https://github.com/nlohmann/json.git
          GIT_TAG v3.9.1

          OPTIONS
          "JSON_BuildTests OFF"
          "JSON_Install OFF")

env_fetch(immutable
          GIT_REPOSITORY https://github.com/Hrle97/immer.git
          GIT_TAG cmake

          OPTIONS
          "immer_BUILD_TESTS OFF"
          "immer_BUILD_EXAMPLES OFF"
          "immer_BUILD_DOCS OFF"
          "immer_BUILD_EXTRAS OFF")

env_fetch(transducers
          GIT_REPOSITORY https://github.com/Hrle97/immer.git
          GIT_TAG cmake

          OPTIONS
          "ZUG_BUILD_TESTS OFF"
          "ZUG_BUILD_EXAMPLES OFF"
          "ZUG_BUILD_DOCS OFF")


env_log(-!- Adding env dependencies... -!-)

env_add_dep(boost hana)
env_add_dep(test gtest gmock)
env_add_dep(test_main gtest_main gmock)
env_add_dep(bench benchmark)
env_add_dep(json nlohmann_json::nlohmann_json)
env_add_dep(functional zug immer)


env_log(-!- Adding env targets... -!-)

env_project_targets(
        DEPENDENCIES
        env::macros_dep
        env::boost_dep
        env::alloc_dep
        env::json_dep
        env::functional_dep

        TEST_DEPENDENCIES
        env::test_dep

        BENCHMARK_DEPENDENCIES
        env::bench_dep)
