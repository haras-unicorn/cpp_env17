include_guard()

# Project ---------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20...3.22)

project(
  env
  DESCRIPTION "Environment for all of my C++ projects."
  VERSION 0.2.0
  LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnvFunc)

env_project_default_options()

# Fetch -----------------------------------------------------------------------

env_fetch(
  test
  FROM
  GIT_REPOSITORY
  https://github.com/google/googletest.git
  GIT_TAG
  release-1.11.0
  SCAFFOLD
  OPTIONS
  "INSTALL_GTEST OFF"
  "gtest_force_shared_crt ON"
  "gtest_build_tests OFF"
  "gtest_build_examples OFF"
  "gmock_build_tests OFF")

env_fetch(
  benchmark
  FROM
  GIT_REPOSITORY
  https://github.com/google/benchmark.git
  GIT_TAG
  v1.6.0
  SCAFFOLD
  OPTIONS
  "BENCHMARK_ENABLE_TESTING OFF"
  "BENCHMARK_ENABLE_INSTALL OFF"
  "BENCHMARK_ENABLE_GTEST_TESTS OFF")

# Dependencies ----------------------------------------------------------------

env_add_dep(test gtest gmock)
env_add_dep(test_main gtest_main gmock)
env_add_dep(bench benchmark)

# Targets ---------------------------------------------------------------------

env_project_targets(SHARE_PCH TEST_DEPS env::test BENCH_DEPS env::bench)
