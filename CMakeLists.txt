include_guard()


# Project ---------------------------------------------------------------------

cmake_minimum_required(VERSION 3.19...3.20)

project(env
        DESCRIPTION "Environment for all of my C++ projects."
        VERSION 1.0
        LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnvFunc)

env_project_default_options()


# Fetch -----------------------------------------------------------------------

env_fetch(macros
          FROM
          GIT_REPOSITORY https://github.com/nemequ/hedley.git
          GIT_TAG v15

          SCAFFOLD
          INCLUDE_DIRS "/")

env_fetch(hana
          FROM
          GIT_REPOSITORY https://github.com/Hrle97/hana.git
          GIT_TAG cmake

          SCAFFOLD
          OPTIONS
          "BOOST_HANA_BUILD_TESTS OFF"
          "BOOST_HANA_BUILD_EXAMPLES OFF"
          "BOOST_HANA_BUILD_BENCHMARKS OFF"
          "BOOST_HANA_BUILD_DOCS OFF"
          "BOOST_HANA_BUILD_EXTRAS OFF")

env_fetch(test
          FROM
          GIT_REPOSITORY https://github.com/google/googletest.git
          GIT_TAG release-1.10.0

          SCAFFOLD
          OPTIONS
          "INSTALL_GTEST OFF"
          "gtest_force_shared_crt ON"
          "gtest_build_tests OFF"
          "gtest_build_examples OFF"
          "gmock_build_tests OFF")

env_fetch(benchmark
          FROM
          GIT_REPOSITORY https://github.com/google/benchmark.git
          GIT_TAG v1.5.2

          SCAFFOLD
          OPTIONS
          "BENCHMARK_ENABLE_TESTING OFF"
          "BENCHMARK_ENABLE_INSTALL OFF"
          "BENCHMARK_ENABLE_GTEST_TESTS OFF")

env_fetch(alloc
          FROM
          GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
          GIT_TAG v2.0.0

          SCAFFOLD
          OPTIONS
          "MI_USE_CXX ON"
          "MI_BUILD_SHARED OFF"
          "MI_BUILD_OBJECT OFF"
          "MI_BUILD_TESTS OFF")

env_fetch(json
          FROM
          GIT_REPOSITORY https://github.com/nlohmann/json.git
          GIT_TAG v3.9.1

          SCAFFOLD
          OPTIONS
          "JSON_BuildTests OFF"
          "JSON_Install OFF")

env_fetch(immutable
          FROM
          GIT_REPOSITORY https://github.com/Hrle97/immer.git
          GIT_TAG cmake

          SCAFFOLD
          OPTIONS
          "immer_BUILD_TESTS OFF"
          "immer_BUILD_EXAMPLES OFF"
          "immer_BUILD_DOCS OFF"
          "immer_BUILD_EXTRAS OFF")

env_fetch(transducers
          FROM
          GIT_REPOSITORY https://github.com/Hrle97/zug.git
          GIT_TAG v0.0.9-cmake

          SCAFFOLD
          OPTIONS
          "ZUG_BUILD_TESTS OFF"
          "ZUG_BUILD_EXAMPLES OFF"
          "ZUG_BUILD_DOCS OFF"
          "ZUG_LINK_BOOST OFF")


# Dependencies ----------------------------------------------------------------

env_add_dep(boost hana)
env_add_dep(alloc mimalloc-static)
env_add_dep(test gtest gmock)
env_add_dep(test_main gtest_main gmock)
env_add_dep(bench benchmark)
env_add_dep(json nlohmann_json::nlohmann_json)
env_add_dep(functional zug immer)


# Targets ---------------------------------------------------------------------

env_project_targets(
        SHARE_PCH

        DEPS
        env::macros
        env::boost
        env::alloc
        env::json
        env::functional

        TEST_DEPS
        env::test

        BENCH_DEPS
        env::bench)
