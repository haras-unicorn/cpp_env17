include_guard()

cmake_minimum_required(VERSION 3.17...3.20)


# project

project(env
        DESCRIPTION "Environment for all of my C++ projects."
        VERSION 1.0
        LANGUAGES C CXX)

macro(env_project_set_labels)
    string(TOUPPER ${PROJECT_NAME} UPPER_PROJECT_NAME)
    string(REGEX REPLACE
           [[\.|-|_|/|\\]] [[_]]
           UPPER_PROJECT_NAME
           ${UPPER_PROJECT_NAME})

    string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
    string(REGEX REPLACE
           [[\.|-|_|/|\\]] [[_]]
           LOWER_PROJECT_NAME
           ${LOWER_PROJECT_NAME})
endmacro()

env_project_set_labels()


# options

macro(env_project_set_options)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        option(${UPPER_PROJECT_NAME}_COMPILER_MESSAGES
               "Turn on compiler messages for ${PROJECT_NAME}."
               ON)

    else ()
        option(${UPPER_PROJECT_NAME}_COMPILER_MESSAGES
               "Turn on compiler messages for ${PROJECT_NAME}."
               OFF)

    endif ()

    option(${UPPER_PROJECT_NAME}_BUILD_EXAMPLES
           "Build ${PROJECT_NAME} examples."
           OFF)

    option(${UPPER_PROJECT_NAME}_BUILD_TESTS
           "Build ${PROJECT_NAME} tests."
           OFF)

    option(${UPPER_PROJECT_NAME}_BUILD_DOCS
           "Build ${PROJECT_NAME} docs."
           OFF)

    option(${UPPER_PROJECT_NAME}_BUILD_BENCHMARKS
           "Build ${PROJECT_NAME} benchmarks."
           OFF)

    option(${UPPER_PROJECT_NAME}_BUILD_EXTRAS
           "Build ${PROJECT_NAME} extras."
           OFF)
endmacro()

env_project_set_options()


# logging

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    option(ENV_LOG_VERBOSE
           "Turn on verbose CMake logging."
           ON)
else ()
    option(ENV_LOG_VERBOSE
           "Turn on verbose CMake logging."
           OFF)
endif ()

set(__env_verbose_message_levels
    "\
STATUS;VERBOSE;DEBUG;TRACE\
")

set(__env_message_levels
    "\
FATAL_ERROR;SEND_ERROR;WARNING;AUTHOR_WARNING;DEPRECATION;NOTICE;\
${env_verbose_message_levels}\
")

if (ENV_LOG_VERBOSE)
    function(env_log _level)
        list(FIND __env_message_levels ${_level} _index)

        list(JOIN ARGN " " _message)
        if (NOT _index EQUAL -1)
            message(${_level}
                    "[env::${LOWER_PROJECT_NAME}]: ${_message}")
        else ()
            message(STATUS
                    "[env::${LOWER_PROJECT_NAME}]: ${_level} ${_message}")
        endif ()
    endfunction()
else ()
    function(env_log _level)
        list(FIND __env_verbose_message_levels ${_level} _index)

        list(JOIN ARGN " " _message)
        if (NOT _index EQUAL -1)
            message(${_level}
                    "[env::${LOWER_PROJECT_NAME}]: ${_message}")
        endif ()
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

function(env_target_name_for _path _out)
    string(REGEX MATCH [[\.?.*$]] _extension "${_path}")
    file(RELATIVE_PATH _relative "${CMAKE_CURRENT_SOURCE_DIR}" "${_path}")

    string(REPLACE ${_extension} "" _name "${_relative}")
    string(REGEX REPLACE / _ _name "${_name}")

    set(${_out} ${LOWER_PROJECT_NAME}_${_name} PARENT_SCOPE)
endfunction()


# compiler

env_log(Compiler ID is: \"${CMAKE_CXX_COMPILER_ID}\".)
env_log(MSVC is present: ${MSVC})

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    if (MSVC)
        env_log(Detected ClangCl compiler.)
        set(ENV_CLANG_CL TRUE CACHE BOOL "Whether CLANG_CL was detected or not.")
        set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
        set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
        set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")
    else ()
        env_log(Detected Clang compiler.)
        set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
        set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
        set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
        set(ENV_CLANG TRUE CACHE BOOL "Whether Clang was detected or not.")
    endif ()

elseif (MSVC) # for some reason "CMAKE_CXX_COMPILER_ID STREQUAL MSVC" doesn't work
    env_log(Detected MSVC compiler.)
    set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
    set(ENV_MSVC TRUE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    env_log(Detected GCC compiler.)
    set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
    set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC TRUE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

else ()
    env_log(Unknown compiler.)
    set(ENV_CLANG_CL FALSE CACHE BOOL "Whether CLANG_CL was detected or not.")
    set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

endif ()


# sources

function(env_target_link _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(Linking \"${_name}\" with \"${ARGN}\".)

    target_link_libraries(${_mod} ${ARGN})
endfunction()

function(env_target_include _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(Into \"${_name}\" including \"${ARGN}\".)

    target_include_directories(${_mod} ${ARGN})
endfunction()

function(env_target_sources _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(Sourcing \"${_name}\" with \"${ARGN}\".)

    target_sources(${_mod} ${ARGN})
endfunction()


# properties

function(env_target_set _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(On \"${_name}\" setting \"${ARGN}\".)

    set_target_properties(${_mod} PROPERTIES ${ARGN})
endfunction()

include(CheckIPOSupported)
check_ipo_supported(RESULT env_ipo_supported)
if (env_ipo_supported)
    env_log(Interprocedural optimization is supported.)

    function(env_target_set_ipo _name)
        env_target_set(${_name} INTERPROCEDURAL_OPTIMIZATION ON)
    endfunction()
else ()
    env_log(Interprocedural optimization is not supported.)

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
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(On \"${_name}\" adding link options \"${ARGN}\".)

    target_link_options(${_mod} ${ARGN})
endfunction()

include(CheckCXXCompilerFlag)
function(env_target_safely_compile_with _name _visibility)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(On \"${_name}\" adding compile options \"${ARGN}\".)

    foreach (_flag IN LISTS ARGN)
        check_compiler_flag(CXX ${_flag} _supported)
        if (_supported)
            target_compile_options(${_mod} ${_visibility} ${_flag})
        else ()
            env_log(WARNING
                    On \"${_name}\" adding compile option \"${_flag}\"
                    FAILED. REASON: Option not supported by compiler.)
        endif ()
    endforeach ()
endfunction()


# compilation

function(env_target_precompile _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(Precompiling \"${_name}\" with \"${ARGN}\".)

    target_precompile_headers(${_mod} ${ARGN})
endfunction()

function(env_target_features _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(Compiling \"${_name}\" with \"${ARGN}\".)

    target_compile_features(${_mod} ${ARGN})
endfunction()

function(env_target_definitions _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(Compiling \"${_name}\" with \"${ARGN}\".)

    target_compile_definitions(${_mod} ${ARGN})
endfunction()

function(env_set_cpp17 _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log(Setting C++17 standard on \"${_name}\".)

    target_compile_features(${_mod} PRIVATE cxx_std_17)
    set_target_properties(${_mod} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()


# warnings

if (ENV_CLANG_CL)
    function(env_target_warn _name)
        env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
        env_log(Adding warnings to \"${_name}\".)

        target_compile_options(
                ${_mod}
                PRIVATE
                /W4 /WX
                --analyze
                # standards compliance
                /permissive-
                # otherwise we can't detect the C++ standard
                /Zc:__cplusplus)
    endfunction()
    function(env_import_warn _name)
        env_log(Removing warnings from \"${_name}\".)

        target_compile_options(${_name} PRIVATE /w)
    endfunction()
elseif (ENV_MSVC)
    function(env_target_warn _name)
        env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
        env_log(Adding warnings to \"${_name}\".)

        target_compile_options(
                ${_mod}
                PRIVATE
                /W4 /WX
                /analyze
                # standards compliance
                /permissive-
                # otherwise we can't detect the C++ standard
                /Zc:__cplusplus)
    endfunction()
    function(env_import_warn _name)
        env_log(Removing warnings from \"${_name}\".)

        target_compile_options(${_name} PRIVATE /w)
    endfunction()
elseif (ENV_GCC)
    function(env_target_warn _name)
        env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_mod}
                PRIVATE
                -Wall -Wextra -Wpedantic -Werror
                -fanalyzer
                # so messages are printed nicely
                -ftrack-macro-expansion=0
                # detect endianness
                -Wno-multichar)
    endfunction()
    function(env_import_warn _name)
        env_log(Removing warnings from \"${_name}\".)

        target_compile_options(${_name} PRIVATE -w)
    endfunction()
elseif (ENV_CLANG)
    function(env_target_warn _name)
        env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
        env_log(Adding warnings to \"${_name}\".)

        target_compile_options(
                ${_mod}
                PRIVATE
                -Wall -Wextra -Wpedantic -Werror
                --analyze)
    endfunction()
    function(env_import_warn _name)
        env_log(Removing warnings from \"${_name}\".)

        target_compile_options(${_name} PRIVATE -w)
    endfunction()
else ()
    function(env_target_warn)
    endfunction()
    function(env_import_warn)
    endfunction()
endif ()


# optimization

if (CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    if (ENV_CLANG_CL)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding optimizations to \"${_name}\".)

            target_compile_options(${_mod} PRIVATE /O2)
            env_target_set_ipo(${_name})
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding optimizations to \"${_name}\".)

            target_compile_options(${_mod} PRIVATE /O2)
            env_target_set_ipo(${_mod})
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding optimizations to \"${_name}\".)

            target_compile_options(${_mod} PRIVATE -O3)
            env_target_set_ipo(${_mod})
        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding optimizations to \"${_name}\".)

            target_compile_options(${_mod} PRIVATE -O3)
            env_target_set_ipo(${_mod})
        endfunction()
    else ()
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding optimizations to \"${_name}\".)

            env_target_set_ipo(${_mod})
        endfunction()
    endif ()
else ()
    if (ENV_CLANG_CL)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding sanitization to \"${_name}\".)

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    /Zi
                    # TODO
                    # -fsanitize=address,undefined
                    # /fsanitize=address
            )
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding sanitization to \"${_name}\".)

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    /Zi
                    /fsanitize=address)
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding sanitization to \"${_name}\".)

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    -Og
                    -ggdb
                    -fsanitize=address,leak,undefined)
        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
            env_log(Adding sanitization to \"${_name}\".)

            target_compile_options(
                    ${_mod}
                    PRIVATE
                    -ggdb
                    # TODO: fix
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

set(__env_fetch_dir "${PROJECT_SOURCE_DIR}/.fetch")
set(__env_fetch_build_dir "${PROJECT_BINARY_DIR}/.fetch")

set(FETCHCONTENT_BASE_DIR ${__env_fetch_dir})

function(env_fetch _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_suffix(${_mod} fetch _mod)

    set(_src_dir "${__env_fetch_dir}/${_name}")
    set(_bin_dir "${__env_fetch_build_dir}/${_name}/bin")
    set(_sub_dir "${__env_fetch_build_dir}/${_name}/sub")
    set(_populated_file "${__env_fetch_dir}/.process/${_name}.populated")
    set(_lock_file "${__env_fetch_dir}/.process/${_name}.lock")

    set(${_mod}_src_dir "${_src_dir}" PARENT_SCOPE)
    set(${_mod}_bin_dir "${_bin_dir}" PARENT_SCOPE)
    set(${_mod}_sub_dir "${_src_sir}" PARENT_SCOPE)

    file(LOCK "${_lock_file}")
    if (NOT EXISTS "${_populated_file}")
        file(WRITE "${_populated_file}" YES)

        env_log("Fetching \"${_name}\" into \"${_src_dir}\".")
        fetchcontent_populate(
                ${_mod}
                QUIET
                ${ARGN}
                SOURCE_DIR "${_src_dir}"
                BINARY_DIR "${_bin_dir}"
                SUBBUILD_DIR "${_sub_dir}")
    else ()
        env_log("Already fetched \"${_name}\" into \"${_src_dir}\".")
    endif ()
    file(LOCK "${_lock_file}" RELEASE)

    file(GLOB _is_sub "${_src_dir}/CMakeLists.txt")
    if (_is_sub)
        env_log(Adding ${_name} subdirectories \"${_src_dir}\", \"${_bin_dir}\".)

        set(_previous_log_level ${CMAKE_MESSAGE_LOG_LEVEL})
        set(_previous_log_indent ${CMAKE_MESSAGE_INDENT})

        set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
        set(CMAKE_MESSAGE_INDENT "${_previous_log_indent}    ")
        add_subdirectory("${_src_dir}" "${_bin_dir}")

        set(CMAKE_MESSAGE_LOG_LEVEL ${_previous_log_level})
        set(CMAKE_MESSAGE_INDENT ${_previous_log_indent})
    endif ()
endfunction()


# atomic targets

function(env_add_executable _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log("Adding executable \"${_name}\".")

    add_executable(${_mod} ${ARGN})

    env_target_link(${_mod} PRIVATE env::default_dep)
    env_target_link(${_mod} PRIVATE ${LOWER_LOWER_PROJECT_NAME}::pch)
    env_target_include(${_mod} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    env_target_warn(${_mod})
    env_target_optimize(${_mod})

    env_set_cpp17(${_mod})
endfunction()

function(env_add_library _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log("Adding library \"${_name}\".")

    add_library(${_mod} ${ARGN})

    env_target_link(${_mod} PRIVATE env::default_dep)
    env_target_link(${_mod} PRIVATE ${LOWER_LOWER_PROJECT_NAME}::pch)
    env_target_include(${_mod} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    env_target_warn(${_mod})
    env_target_optimize(${_mod})

    env_target_set_pie(${_mod})
    env_set_cpp17(${_mod})
endfunction()

function(env_add_interface _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log("Adding interface \"${_name}\".")

    add_library(${_mod} INTERFACE)

    env_target_link(${_mod} INTERFACE ${ARGN})
endfunction()

function(env_add_import _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log("Adding import \"${_name}\".")

    add_library(${_mod} INTERFACE IMPORTED GLOBAL)

    env_target_link(${_mod} INTERFACE ${ARGN})
endfunction()

function(env_add_alias _name)
    env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
    env_log("Adding alias \"${LOWER_PROJECT_NAME}::${_name}\".")

    add_library(${LOWER_PROJECT_NAME}::${_name} ALIAS ${_mod})
endfunction()

function(env_project_add_pch)
    env_suffix(${LOWER_PROJECT_NAME} pch _mod)
    env_log(" - Adding precompiled headers of \"${PROJECT_NAME}\". - ")

    set(_source "${CMAKE_CURRENT_SOURCE_DIR}/pch/${LOWER_PROJECT_NAME}_pch.cpp")
    set(_header "${CMAKE_CURRENT_SOURCE_DIR}/pch/${LOWER_PROJECT_NAME}_pch.hpp")

    add_library(${_mod} STATIC ${_source})
    add_library(${LOWER_PROJECT_NAME}::${pch} ALIAS ${_mod})

    env_target_precompile(${_mod} PUBLIC ${_header})

    env_import_warn(${_mod})
    env_target_optimize(${_mod})

    env_target_set_pie(${_mod})
    env_set_cpp17(${_mod})
endfunction()


# compound targets

function(env_add_dep _name)
    env_suffix(${_name} dep _mod)
    env_log(" - Adding dependency \"${_name}\". - ")

    env_add_import(${_mod} ${ARGN})
    env_add_alias(${_mod})
endfunction()

if (ENV_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)

    function(env_add_test _name _src)
        env_suffix(${_name} test _mod)
        env_log(" - Adding test \"${_name}\". - ")

        env_add_executable(${_mod} ${_src})

        env_prefix(${_mod} ${LOWER_PROJECT_NAME} _mod)
        gtest_discover_tests(${_mod})
    endfunction()
else ()
    function(env_add_test)
    endfunction()
endif ()

if (NOT CMAKE_BUILD_TYPE STREQUAL Debug AND ENV_BUILD_BENCHMARKS)
    function(env_add_bench _name)
        env_suffix(${_name} bench _mod)
        env_log(" - Adding bench \"${_name}\". - ")

        env_add_executable(${_mod} ${ARGN})
    endfunction()
else ()
    function(env_add_bench)
    endfunction()
endif ()

function(env_add_static _name)
    env_suffix(${_name} static _mod)
    env_log(" - Adding static \"${_name}\". - ")

    env_add_library(${_mod} STATIC ${ARGN})
    env_add_alias(${_mod})
endfunction()

function(env_add_shared _name)
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


# aggregate targets

function(env_hook _dependency)
    cmake_parse_arguments(ENV_HOOK "" "" "INTO" ${ARGN})

    foreach (_target IN LISTS ENV_HOOK_INTO)
        if (NOT TARGET ${_target})
            add_custom_target(${_target} DEPENDS ${_dependency})
        else ()
            get_target_property(
                    _dependencies
                    ${_target}
                    MANUALLY_ADDED_DEPENDENCIES)

            list(FIND _dependencies ${_dependency} _index)
            if (_index EQUAL -1)
                add_dependencies(${_target} ${_dependency})
            endif ()
        endif ()
    endforeach ()
endfunction()


# default dependencies

env_log("-!- Adding default dependencies... -!-")


env_fetch(macros
          GIT_REPOSITORY https://github.com/nemequ/hedley.git
          GIT_TAG v15)

env_add_dep(macros)

env_target_include(macros_dep INTERFACE ${env_macros_fetch_src_dir})


set(BOOST_HANA_BUILD_TESTS OFF CACHE BOOL
    "Don't build Hana tests."
    FORCE)

set(BOOST_HANA_BUILD_EXAMPLES OFF CACHE BOOL
    "Don't build Hana examples."
    FORCE)

set(BOOST_HANA_BUILD_BENCHMARKS OFF CACHE BOOL
    "Don't build Hana benchmarks."
    FORCE)

set(BOOST_HANA_BUILD_DOCS OFF CACHE BOOL
    "Don't build Hana docs."
    FORCE)

set(BOOST_HANA_BUILD_EXTRAS OFF CACHE BOOL
    "Don't build Hana extras."
    FORCE)

env_fetch(hana
          GIT_REPOSITORY https://github.com/Hrle97/hana.git
          GIT_TAG cmake)

env_add_dep(boost hana)


set(INSTALL_GTEST OFF CACHE BOOL
    "Don't install GoogleTest."
    FORCE)

set(gtest_force_shared_crt ON CACHE BOOL
    "Use same compiler/linker as project."
    FORCE)

set(gtest_build_tests OFF CACHE BOOL
    "Don't build GoogleTest tests."
    FORCE)

set(gtest_build_samples OFF CACHE BOOL
    "Don't build GoogleTest samples."
    FORCE)

set(gmock_build_tests CACHE BOOL
    "Don't build GoogleMock tests."
    FORCE)

env_fetch(test
          GIT_REPOSITORY https://github.com/google/googletest.git
          GIT_TAG release-1.10.0)

env_add_dep(test gtest gmock)

env_add_dep(test_main gtest_main gmock)


set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL
    "Disable tests on GoogleBenchmark."
    FORCE)

set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL
    "Disable install on GoogleBenchmark."
    FORCE)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL
    "Disable tests depending on GoogleTest for GoogleBenchmark."
    FORCE)

env_fetch(bench
          GIT_REPOSITORY https://github.com/google/benchmark.git
          GIT_TAG v1.5.2)

env_add_dep(bench benchmark)


set(MI_USE_CXX ON CACHE BOOL
    "Compile Mimalloc using C++ compiler."
    FORCE)

set(MI_BUILD_SHARED OFF CACHE BOOL
    "Don't compile Mimalloc as a shared library."
    FORCE)

set(MI_BUILD_OBJECT OFF CACHE BOOL
    "Don't compile Mimalloc as an object library."
    FORCE)

set(MI_BUILD_TESTS OFF CACHE BOOL
    "Don't compile Mimalloc tests."
    FORCE)

env_fetch(alloc
          GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
          GIT_TAG v2.0.0)

env_add_dep(alloc mimalloc-static)


set(JSON_BuildTests OFF CACHE BOOL
    "Don't build tests for Nlohmann Json."
    FORCE)

set(JSON_Install OFF CACHE BOOL
    "Don't install Nlohmann Json."
    FORCE)

env_fetch(json
          GIT_REPOSITORY https://github.com/nlohmann/json.git
          GIT_TAG v3.9.1)

env_add_dep(json nlohmann_json::nlohmann_json)


set(immer_BUILD_EXAMPLES OFF CACHE BOOL
    "Don't build immer examples."
    FORCE)

set(immer_BUILD_TESTS OFF CACHE BOOL
    "Don't build immer tests."
    FORCE)

set(immer_BUILD_DOCS OFF CACHE BOOL
    "Don't build immer docs."
    FORCE)

set(immer_BUILD_EXTRAS OFF CACHE BOOL
    "Don't build immer extras."
    FORCE)

env_fetch(immutable
          GIT_REPOSITORY https://github.com/Hrle97/immer.git
          GIT_TAG cmake)

set(ZUG_BUILD_TESTS OFF CACHE BOOL
    "Don't build zug tests."
    FORCE)

set(ZUG_BUILD_EXAMPLES OFF CACHE BOOL
    "Don't build zug examples."
    FORCE)

set(ZUG_BUILD_DOCS OFF CACHE BOOL
    "Don't build zug docs."
    FORCE)

env_fetch(transducers
          GIT_REPOSITORY https://github.com/Hrle97/zug.git
          GIT_TAG v0.0.2-cmake)

env_add_dep(functional zug immer)


env_add_dep(default)

env_target_link(
        default_dep
        INTERFACE
        env::macros_dep
        env::boost_dep
        env::test_dep
        env::bench_dep
        env::alloc_dep
        env::json_dep
        env::functional_dep)


# targets

env_log("-!- Adding targets... -!-")

env_project_add_pch()

env_add_test(env src/env_test.cpp)
env_add_bench(env src/env_bench.cpp)

env_add_export(env)
