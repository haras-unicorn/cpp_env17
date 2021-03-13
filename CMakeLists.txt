include_guard()

cmake_minimum_required(VERSION 3.15...3.20)
project(fast-web-audio
        DESCRIPTION "Tools for creating low latency audio web apps."
        VERSION 0.1
        LANGUAGES CXX)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${PROJECT_SOURCE_DIR}/cmake_modules
        ${PROJECT_SOURCE_DIR}/cmake_modules/dependencies)

include(FWATargets)
