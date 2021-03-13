include_guard()

cmake_minimum_required(VERSION 3.11...3.20)
project(
        env
        DESCRIPTION "Environment for all of my C++ projects."
        VERSION 1.0
        LANGUAGES CXX
)

set(
        CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_SOURCE_DIR}/cmake/deps
)


include(ENVDeps)

env_log("-!- Adding targets... -!-")

env_add_test("" ${PROJECT_SOURCE_DIR}/src/env_test.cpp)
env_add_bench("" ${PROJECT_SOURCE_DIR}/src/env_bench.cpp)
