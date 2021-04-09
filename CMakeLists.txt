include_guard()


# project

cmake_minimum_required(VERSION 3.17...3.20)

project(
        env
        DESCRIPTION "Environment for all of my C++ projects."
        VERSION 1.0
        LANGUAGES C CXX
)


# options

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    option(ENV_LOG "Turn on CMake logging." ON)
    option(ENV_MESSAGE "Turn on compiler messages." ON)
else ()
    option(ENV_LOG "Turn on CMake logging." OFF)
    option(ENV_MESSAGE "Turn on compiler messages." OFF)
endif ()

option(ENV_BUILD_EXAMPLES "Build examples." OFF)


# log

if (ENV_LOG)
    function(env_log)
        message(STATUS "[env::${PROJECT_NAME}]: ${ARGN}")
    endfunction()
else ()
    function(env_log)
    endfunction()
endif ()


# names

function(env_prefix _name _prefix _out)
    string(REGEX MATCH "^${_prefix}" _match ${_name})
    if (_name AND NOT _match)
        set(${_out} "${_prefix}_${_name}" PARENT_SCOPE)
    elseif (_match)
        set(${_out} "${_name}" PARENT_SCOPE)
    else ()
        set(${_out} "${_prefix}" PARENT_SCOPE)
    endif ()
endfunction()

function(env_suffix _name _suffix _out)
    string(REGEX MATCH "${_suffix}\$" _match ${_name})
    if (_name AND NOT _match)
        set(${_out} "${_name}_${_suffix}" PARENT_SCOPE)
    elseif (_match)
        set(${_out} "${_name}" PARENT_SCOPE)
    else ()
        set(${_out} "${_suffix}" PARENT_SCOPE)
    endif ()
endfunction()


# compiler

env_log("Compiler ID is: \"${CMAKE_CXX_COMPILER_ID}\".")

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    if (MSVC)
        env_log("Detected ClangCl compiler.")
        set(ENV_CLANG_CL TRUE CACHE BOOL "Whether CLANG_CL was detected or not.")
        set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
        set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
        set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")
    else ()
        env_log("Detected Clang compiler.")
        set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
        set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
        set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
        set(ENV_CLANG TRUE CACHE BOOL "Whether Clang was detected or not.")
    endif ()

elseif (MSVC) # for some reason "CMAKE_CXX_COMPILER_ID STREQUAL MSVC" doesn't work
    env_log("Detected MSVC compiler.")
    set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
    set(ENV_MSVC TRUE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    env_log("Detected GCC compiler.")
    set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
    set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC TRUE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

else ()
    env_log("Unknown compiler.")
    set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
    set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

endif ()


# sources

function(env_target_link _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Linking \"${_name}\" with \"${ARGN}\".")

    target_link_libraries(${_mod} ${ARGN})
endfunction()

function(env_target_include _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Into \"${_name}\" including \"${ARGN}\".")

    target_include_directories(${_mod} ${ARGN})
endfunction()

function(env_target_sources _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Sourcing \"${_name}\" with \"${ARGN}\".")

    target_sources(${_mod} ${ARGN})
endfunction()


# properties

function(env_target_set _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("On \"${_name}\" setting \"${ARGN}\".")

    set_target_properties(${_mod} PROPERTIES ${ARGN})
endfunction()

include(CheckIPOSupported)
check_ipo_supported(RESULT env_ipo_supported)
if (env_ipo_supported)
    env_log("Interprocedural optimization is supported.")

    function(env_target_set_ipo _name)
        env_target_set(${_name} INTERPROCEDURAL_OPTIMIZATION ON)
    endfunction()
else ()
    env_log("Interprocedural optimization is not supported.")

    function(env_target_set_ipo _name)
    endfunction()
endif ()

include(CheckPIESupported)
check_pie_supported(OUTPUT_VARIABLE env_pie_supported LANGUAGES CXX)
if (env_pie_supported)
    env_log("Position independent code is supported.")

    function(env_target_set_pie _name)
        env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
    endfunction()
else ()
    function(env_target_set_pie _name)
    endfunction()
endif ()


# flags

function(env_target_link_with _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("On \"${_name}\" adding link options \"${ARGN}\".")

    target_link_options(${_mod} ${ARGN})
endfunction()

include(CheckCXXCompilerFlag)
function(env_target_safely_compile_with _name _visibility)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("On \"${_name}\" adding compile options \"${ARGN}\".")

    foreach (_flag IN LISTS ARGN)
        check_compiler_flag(CXX ${_flag} _supported)
        if (_supported)
            target_compile_options(${_mod} ${_visibility} ${_flag})
        else ()
            env_log(
                    "WARN: On \"${_name}\" adding compile option \"${_flag}\" \
                    FAILED. REASON: Option not supported by compiler."
            )
        endif ()
    endforeach ()
endfunction()


# compilation

function(env_target_precompile _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Precompiling \"${_name}\" with \"${ARGN}\".")

    target_precompile_headers(${_mod} ${ARGN})
endfunction()

function(env_target_features _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")

    target_compile_features(${_mod} ${ARGN})
endfunction()

function(env_target_definitions _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")

    target_compile_definitions(${_mod} ${ARGN})
endfunction()

function(env_set_cpp17 _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Setting C++17 standard on \"${_name}\".")

    target_compile_features(${_mod} PRIVATE cxx_std_17)
    set_target_properties(${_mod} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()


# warnings

if (ENV_CLANG_CL)
    function(env_target_warn _name)
        env_prefix(${_name} ${PROJECT_NAME} _mod)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_mod}
                PRIVATE
                /W4 /WX
                # used separately and would probably take a long time
                # --analyze
                /permissive- # standards compliance
                /Zc:__cplusplus # otherwise we can't detect the C++ standard
        )
    endfunction()
elseif (ENV_MSVC)
    function(env_target_warn _name)
        env_prefix(${_name} ${PROJECT_NAME} _mod)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_mod}
                PRIVATE
                /W4 /WX
                /analyze
                /permissive- # standards compliance
                /Zc:__cplusplus # otherwise we can't detect the C++ standard
        )
    endfunction()
elseif (ENV_GCC)
    function(env_target_warn _name)
        env_prefix(${_name} ${PROJECT_NAME} _mod)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_mod}
                PRIVATE
                -Wall -Wextra -Wpedantic -Werror
                # takes too long - run when library is done?
                # -fanalyzer
                -ftrack-macro-expansion=0 # so messages are printed nicely
                -Wno-multichar # detect endianness
        )
    endfunction()
elseif (ENV_CLANG)
    function(env_target_warn _name)
        env_prefix(${_name} ${PROJECT_NAME} _mod)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_mod}
                PRIVATE
                # used separately and would probably take a long time
                # --analyze
                -Wall -Wextra -Wpedantic -Werror
        )
    endfunction()
else ()
    function(env_target_warn _name)
    endfunction()
endif ()


# optimization

if (CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    if (ENV_CLANG_CL)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    /O2
            )

            env_target_set_ipo(${_name})
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    /O2
            )

            env_target_set_ipo(${_mod})
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    -O3
            )

            env_target_set_ipo(${_mod})

        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    -O3
            )

            env_target_set_ipo(${_mod})
        endfunction()
    else ()
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding optimizations to \"${_name}\".")

            env_target_set_ipo(${_mod})
        endfunction()
    endif ()
else ()
    if (ENV_CLANG_CL)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    # TODO: fix ClangCL complains
                    # /ZI # debug info
                    # /fsanitize=address
            )
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    /ZI # debug info
                    /fsanitize=address
            )
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    -Og
                    -ggdb
                    -fsanitize=address,leak,undefined
            )
        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} ${PROJECT_NAME} _mod)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    -ggdb
                    # TODO fix
                    # -fsanitize=address,undefined
            )
        endfunction()
    else ()
        function(env_target_optimize _name)
        endfunction()
    endif ()
endif ()


# fetch

include(FetchContent)

set(env_fetch_dir "${PROJECT_SOURCE_DIR}/.fetch")
set(env_fetch_build_dir "${PROJECT_BINARY_DIR}/.fetch")

set(FETCHCONTENT_BASE_DIR ${env_fetch_dir})


function(env_fetch _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_suffix(${_mod} fetch _mod)

    set(src_dir "${env_fetch_dir}/${_name}_src")
    set(bin_dir "${env_fetch_build_dir}/${_name}/bin")
    set(sub_dir "${env_fetch_build_dir}/${_name}/sub")
    set(populated_file "${env_fetch_dir}/.process/${_name}.populated")
    set(lock_file "${env_fetch_dir}/.process/${_name}.lock")

    set(${_mod}_src_dir "${src_dir}" PARENT_SCOPE)
    set(${_mod}_bin_dir "${bin_dir}" PARENT_SCOPE)
    set(${_mod}_sub_dir "${src_sir}" PARENT_SCOPE)

    file(LOCK "${lock_file}")
    if (NOT EXISTS "${populated_file}")
        file(WRITE "${populated_file}" YES)

        env_log("Fetching \"${_name}\" into \"${src_dir}\".")
        fetchcontent_populate(
                ${_mod}
                QUIET
                ${ARGN}
                SOURCE_DIR "${src_dir}"
                BINARY_DIR "${bin_dir}"
                SUBBUILD_DIR "${sub_dir}"
        )
    else ()
        env_log("Already fetched \"${_name}\" into \"${src_dir}\".")
    endif ()
    file(LOCK "${lock_file}" RELEASE)

    file(GLOB is_sub "${src_dir}/CMakeLists.txt")
    if (is_sub)
        env_log("Adding ${_name} subdirectories \"${_src_dir}\", \"${bin_dir}\".")
        set(_previous_log_level ${CMAKE_MESSAGE_LOG_LEVEL})
        set(_previous_log_indent ${CMAKE_MESSAGE_INDENT})
        set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
        set(CMAKE_MESSAGE_INDENT "${_previous_log_indent}    ")

        add_subdirectory("${src_dir}" "${bin_dir}")

        set(CMAKE_MESSAGE_LOG_LEVEL ${_previous_log_level})
        set(CMAKE_MESSAGE_INDENT ${_previous_log_indent})
    endif ()
endfunction()


# atomic targets

function(env_add_executable _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Adding executable \"${_name}\".")

    add_executable(${_mod} ${ARGN})

    env_target_link(${_mod} PRIVATE env::default_dep)
    env_target_include(${_mod} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    env_target_warn(${_mod})
    env_target_optimize(${_mod})

    env_set_cpp17(${_mod})
endfunction()

function(env_add_library _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Adding library \"${_name}\".")

    add_library(${_mod} ${ARGN})

    env_target_link(${_mod} PRIVATE env::default_dep)
    env_target_include(${_mod} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    env_target_warn(${_mod})
    env_target_optimize(${_mod})

    env_target_set_pie(${_mod})
    env_set_cpp17(${_mod})
endfunction()

function(env_add_interface _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Adding interface \"${_name}\".")

    add_library(${_mod} INTERFACE)

    env_target_link(${_mod} INTERFACE ${ARGN})
endfunction()

function(env_add_import _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Adding import \"${_name}\".")

    add_library(${_mod} INTERFACE IMPORTED GLOBAL)

    env_target_link(${_mod} INTERFACE ${ARGN})
endfunction()

function(env_add_alias _name)
    env_prefix(${_name} ${PROJECT_NAME} _mod)
    env_log("Adding alias \"${PROJECT_NAME}::${_name}\".")

    add_library(${PROJECT_NAME}::${_name} ALIAS ${_mod})
endfunction()


# compound targets

function(env_add_dep _name)
    env_suffix(${_name} dep _mod)
    env_log(" - Adding dependency \"${_name}\". - ")

    env_add_import(${_mod} ${ARGN})
    env_add_alias(${_mod})
endfunction()

enable_testing()
include(GoogleTest)
function(env_add_test _name _src)
    env_suffix(${_name} test _mod)
    env_log(" - Adding test \"${_name}\". - ")

    env_add_executable(${_mod} ${_src})

    env_prefix(${_mod} ${PROJECT_NAME} _mod)
    gtest_discover_tests(${_mod})
endfunction()

if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    function(env_add_bench _name)
        env_suffix(${_name} bench _mod)
        env_log(" - Adding bench \"${_name}\". - ")

        env_add_executable(${_mod} ${ARGN})
    endfunction()
else ()
    function(env_add_bench)
    endfunction()
endif ()

function(env_add_static_name)
    env_suffix(${_name} static _mod)
    env_log(" - Adding static \"${_name}\". - ")

    env_add_library(${_mod} STATIC ${ARGN})
    env_add_alias(${_mod})
endfunction()

function(env_add_shared_name)
    env_suffix(${_name} shared _mod)
    env_log(" - Adding shared \"${_name}\". - ")

    env_add_library(${_mod} SHARED ${ARGN})
    env_add_alias(${_mod})
endfunction()

function(env_add_app _name)
    env_suffix(${_name} app _mod)
    env_log(" - Adding app \"${_name}\". - ")

    env_add_executable(${_mod} ${ARGN})
endfunction()

function(env_add_export _name)
    env_suffix(${_name} export _mod)
    env_log(" - Adding export \"${_name}\". - ")

    env_add_interface(${_mod} ${ARGN})
    env_add_alias(${_mod})
endfunction()


# deps

env_log("-!- Adding default dependencies... -!-")


env_fetch(
        macros
        GIT_REPOSITORY https://github.com/nemequ/hedley.git
        GIT_TAG v15
)

env_add_dep(macros)

env_target_include(macros_dep INTERFACE ${env_macros_fetch_src_dir})


env_add_dep(std)

env_target_precompile(
        env_std_dep
        INTERFACE
        <utility>
        <type_traits>

        <memory>
        <new>
        <cstring>
        <iterator>

        <bitset>

        <any>
        <optional>
        <tuple>

        <string>
        <string_view>

        <stack>
        <deque>
        <queue>

        <list>
        <forward_list>

        <array>
        <vector>

        <set>
        <map>
        <unordered_map>
        <unordered_set>

        <thread>
        <mutex>
        <shared_mutex>
        <atomic>
        <chrono>

        <cmath>
        <functional>
        <algorithm>
        <exception>

        <iostream>
        <filesystem>
)


env_fetch(
        hana
        GIT_REPOSITORY https://github.com/boostorg/hana.git
        GIT_TAG v1.7.0
)

env_add_dep(boost hana)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    env_target_definitions(
            boost_dep
            INTERFACE
            BOOST_HANA_CONFIG_ENABLE_DEBUG_MODE
    )
endif ()

env_target_precompile(
        boost_dep
        INTERFACE
        <boost/hana.hpp>
)


set(
        GTEST_FORCE_SHARED_CRT ON CACHE BOOL
        "Use same compiler/linker as project."
        FORCE
)

env_fetch(
        test
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
)

env_add_dep(test gtest gmock)


set(
        BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL
        "Turn off tests for benchmark."
        FORCE
)

env_fetch(
        bench
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.5.2
)

env_add_dep(bench benchmark)


env_fetch(
        alloc
        GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
        GIT_TAG v2.0.0
)

env_add_dep(alloc mimalloc-obj)


env_fetch(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1
)

env_add_dep(json nlohmann_json::nlohmann_json)

env_target_precompile(json_dep INTERFACE <nlohmann/json.hpp>)


env_fetch(
        immutable
        GIT_REPOSITORY https://github.com/Hrle97/immer.git
        GIT_TAG cmake
)

env_fetch(
        transducers
        GIT_REPOSITORY https://github.com/Hrle97/zug.git
        GIT_TAG cmake
)

env_add_dep(functional zug immer)


env_add_dep(default)

env_target_link(
        env_default_dep
        INTERFACE
        env::macros_dep
        env::std_dep
        env::boost_dep
        env::test_dep
        env::bench_dep
        env::alloc_dep
        env::json_dep
        env::functional_dep
)


# targets

env_log("-!- Adding targets... -!-")

env_add_test(env src/env_test.cpp)
env_add_bench(env src/env_bench.cpp)

env_add_export(env)
