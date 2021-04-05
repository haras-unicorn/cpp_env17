include_guard()

cmake_minimum_required(VERSION 3.17...3.20)

project(
        env
        DESCRIPTION "Environment for all of my C++ projects."
        VERSION 1.0
        LANGUAGES C CXX
)


# log

function(env_log)
    message(STATUS "[env]: ${ARGN}")
endfunction()


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
    env_prefix(${_name} env _name)
    env_log("Linking \"${_name}\" with \"${ARGN}\".")

    target_link_libraries(${_name} ${ARGN})
endfunction()

function(env_target_include _name)
    env_prefix(${_name} env _name)
    env_log("Into \"${_name}\" including \"${ARGN}\".")

    target_include_directories(${_name} ${ARGN})
endfunction()

function(env_target_sources _name)
    env_prefix(${_name} env _name)
    env_log("Sourcing \"${_name}\" with \"${ARGN}\".")

    target_sources(${_name} ${ARGN})
endfunction()


# properties

function(env_target_set _name)
    env_prefix(${_name} env _name)
    env_log("On \"${_name}\" setting \"${ARGN}\".")

    set_target_properties(${_name} PROPERTIES ${ARGN})
endfunction()

include(CheckIPOSupported)
check_ipo_supported(RESULT env_ipo_supported)
if (env_ipo_supported)
    env_log("IPO is supported.")

    function(env_target_set_ipo _name)
        env_target_set(${_name} INTERPROCEDURAL_OPTIMIZATION ON)
    endfunction()
else ()
    env_log("IPO is not supported.")

    function(env_target_set_ipo _name)
    endfunction()
endif ()

function(env_target_link_options _name)
    env_prefix(${_name} env _name)
    env_log("On \"${_name}\" adding link options \"${ARGN}\".")

    target_link_options(${_name} PRIVATE ${ARGN})
endfunction()


# compilation

function(env_target_precompile _name)
    env_prefix(${_name} env _name)
    env_log("Precompiling \"${_name}\" with \"${ARGN}\".")

    target_precompile_headers(${_name} ${ARGN})
endfunction()

function(env_target_compile _name)
    env_prefix(${_name} env _name)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")

    target_compile_features(${_name} ${ARGN})
endfunction()

function(env_target_definitions _name)
    env_prefix(${_name} env _name)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")

    target_compile_definitions(${_name} ${ARGN})
endfunction()


# warnings

if (ENV_CLANG_CL)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
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
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
                PRIVATE
                /W4 /WX
                /analyze
                /permissive- # standards compliance
                /Zc:__cplusplus # otherwise we can't detect the C++ standard
        )
    endfunction()
elseif (ENV_GCC)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
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
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
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
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    /O2
            )
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    /O2
            )
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    -O3
            )
        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    -O3
            )
        endfunction()
    else ()
        function(env_target_optimize _name)
        endfunction()
    endif ()
else ()
    if (ENV_CLANG_CL)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    # TODO: fix ClangCL complains
                    # /ZI # debug info
                    # /fsanitize=address
            )
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    /ZI # debug info
                    /fsanitize=address
            )
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    -Og
                    -ggdb
                    -fsanitize=address,leak,undefined
            )
        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
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

set(env_fetch_dir "${PROJECT_SOURCE_DIR}/builds/fetch")
set(env_fetch_build_dir "${PROJECT_BINARY_DIR}/fetch")

set(FETCHCONTENT_BASE_DIR ${env_fetch_dir})


function(env_fetch _name)
    env_prefix(${_name} env _name)
    env_suffix(${_name} fetch _name)

    set(src_dir "${env_fetch_dir}/${_name}_src")
    set(bin_dir "${env_fetch_build_dir}/${_name}/bin")
    set(sub_dir "${env_fetch_build_dir}/${_name}/sub")

    set(lock_file "${env_fetch_dir}/${_name}_populated")

    if (NOT EXISTS "${lock_file}")
        file(WRITE "${lock_file}" YES)
        file(LOCK "${lock_file}")

        env_log("Fetching \"${_name}\" into \"${src_dir}\".")
        fetchcontent_populate(
                ${_name}
                QUIET
                ${ARGN}
                SOURCE_DIR "${src_dir}"
                BINARY_DIR "${bin_dir}"
                SUBBUILD_DIR "${sub_dir}"
        )

        file(LOCK "${lock_file}" RELEASE)
    else ()
        file(LOCK "${lock_file}")

        env_log("Fetched \"${_name}\" into \"${src_dir}\".")

        file(LOCK "${lock_file}" RELEASE)
    endif ()


    set(_previous_log_level ${CMAKE_MESSAGE_LOG_LEVEL})
    set(_previous_log_indent ${CMAKE_MESSAGE_INDENT})
    set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
    set(CMAKE_MESSAGE_INDENT "${_previous_log_indent}    ")

    add_subdirectory("${src_dir}" "${bin_dir}")

    set(CMAKE_MESSAGE_LOG_LEVEL ${_previous_log_level})
    set(CMAKE_MESSAGE_INDENT ${_previous_log_indent})
endfunction()


# atomic targets

function(env_add_executable _name)
    env_prefix(${_name} env _name)
    env_log("Adding \"${_name}\" executable.")

    add_executable(${_name} ${ARGN})
endfunction()

function(env_add_library _name)
    env_prefix(${_name} env _name)

    env_log("Adding \"${_name}\" library.")
    add_library(${_name} ${ARGN})
endfunction()

function(env_add_alias _name)
    env_prefix(${_name} env _prefixed)

    env_log("Adding \"env::${_name}\" alias.")
    add_library(env::${_name} ALIAS ${_prefixed})
endfunction()


# compound targets

function(env_add_dep _name)
    env_suffix(${_name} dep _name)
    env_log(" - Adding \"${_name}\" dependency. - ")

    env_add_library(${_name} INTERFACE IMPORTED)
endfunction()

function(env_add_test _name _src)
    env_suffix(${_name} test _name)
    env_log(" - Adding \"${_name}\" test. - ")

    env_add_executable(${_name} ${_src})
    env_target_link(${_name} PRIVATE ${ARGN} PRIVATE env_default_dep)

    env_target_set(${_name} CXX_EXTENSIONS OFF)
    env_target_link_options(${_name} "-v")
    env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
    env_target_set_ipo(${_name})

    env_target_warn(${_name})
    env_target_optimize(${_name})

    add_test(NAME env_${_name} COMMAND env_${_name})
endfunction()

if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    function(env_add_bench _name _src)
        env_suffix(${_name} bench _name)
        env_log(" - Adding \"${_name}\" benchmark. - ")

        env_add_executable(${_name} ${_src})
        env_target_link(${_name} PRIVATE ${ARGN} PRIVATE env_default_dep)

        env_target_set(${_name} CXX_EXTENSIONS OFF)
        env_target_link_options(${_name} "-v")
        env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
        env_target_set_ipo(${_name})

        env_target_warn(${_name})
        env_target_optimize(${_name})
    endfunction()
else ()
    function(env_add_bench)
    endfunction()
endif ()

function(env_add_export _name)
    env_log(" - Adding \"${_name}\" export. - ")

    env_add_library(${_name} INTERFACE)
    env_add_alias(${_name})
    env_target_link(${_name} INTERFACE ${ARGN} INTERFACE env_default_dep)
    env_target_include(${_name} INTERFACE ${PROJECT_SOURCE_DIR}/include)
endfunction()


# deps

env_log("-!- Adding dependencies... -!-")


env_add_dep(std)
env_target_precompile(
        std_dep
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


env_add_dep(boost)

env_fetch(
        hana
        GIT_REPOSITORY https://github.com/boostorg/hana.git
        GIT_TAG v1.7.0
)

env_target_link(boost_dep INTERFACE hana)

env_target_precompile(
        boost_dep
        INTERFACE
        <boost/hana.hpp>
)


env_add_dep(test)

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

env_target_link(test_dep INTERFACE gtest gmock)


env_add_dep(bench)

set(
        BENCHMARK_ENABLE_GTEST_TESTS FALSE CACHE BOOL
        "Turn off tests for benchmark."
        FORCE
)

env_fetch(
        bench
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.5.2
)

env_target_link(bench_dep INTERFACE benchmark)


env_add_dep(alloc)

env_fetch(
        alloc
        GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
        GIT_TAG v2.0.0
)

env_target_link(alloc_dep INTERFACE mimalloc-obj)


env_add_dep(json)

env_fetch(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1
)

env_target_link(json_dep INTERFACE nlohmann_json::nlohmann_json)


env_add_dep(default)

env_target_include(default_dep INTERFACE ${PROJECT_SOURCE_DIR}/include)

env_target_link(
        default_dep
        INTERFACE
        env_std_dep
        env_boost_dep
        env_test_dep
        env_bench_dep
        env_alloc_dep
        env_json_dep
)

env_target_compile(default_dep INTERFACE cxx_std_17)


# targets

env_log("-!- Adding targets... -!-")

env_add_test(env src/env_test.cpp)
env_add_bench(env src/env_bench.cpp)

env_add_export(env)
