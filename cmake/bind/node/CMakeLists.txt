include_guard()


# Project ---------------------------------------------------------------------

cmake_minimum_required(VERSION 3.17...3.20)

project(env_bind_node
        DESCRIPTION "Environment subproject for creating NodeJS bindings."
        VERSION 1.0
        LANGUAGES C CXX)


# Options ---------------------------------------------------------------------

option(ENV_BIND_NODE_ROOT_DOWNLOAD_URL
       "Root url for downloading NodeJS headers and libraries."
       [[https://nodejs.org/download/release]])

option(ENV_BIND_NODE_VERSION
       "\
Version of NodeJS headers and libraries to download and link against \
bindings."
       "installed")

option(ENV_BIND_NODE_FALLBACK_VERSION
       "\
Version of NodeJS to download when version is set to \"installed\" and \
NodeJS is not installed or it cannot be queried for its version."
       "latest")

option(ENV_BIND_NODE_REMOTE_CHECKSUMS
       "File to download for NodeJS header and library archive checksums."
       "SHASUMS256.txt")

option(ENV_BIND_NODE_CHECK_TYPE
       "Check type to use for NodeJS header and library archive checksums."
       "SHA256")

option(ENV_BIND_NODE_USE_NAN
       "\
Use Native Abstractions for NodeJS instead of node-addon-api for \
abstraction over NodeJS versions. Not recommended."
       OFF)


# Node target -----------------------------------------------------------------

if (NOT TARGET env_bind_node::node)
    env_add_export(node)
endif ()


# Installed NodeJS version ----------------------------------------------------

if (NOT ENV_BIND_NODE_INSTALLED_VERSION)
    find_program(__env_bind_node_binary NAMES node nodejs)

    if (__env_bind_node_binary)
        env_log(NodeJS installation detected.)

        execute_process(
                COMMAND ${__env_bind_node_binary} --version
                RESULT_VARIABLE _res
                OUTPUT_VARIABLE _version
                OUTPUT_STRIP_TRAILING_WHITESPACE)

        if (_res STREQUAL 0)
            set(__env_bind_node_installed_version ${_version})
            env_log(Installed NodeJS version is:
                    \"${__env_bind_node_installed_version}\".)
        else ()
            set(__env_bind_node_installed_version "")
            env_log(Installed NodeJS version not detected.)
        endif ()

        set(ENV_BIND_NODE_INSTALLED_VERSION
            "${__env_bind_node_installed_version}"
            CACHE STRING
            "Installed NodeJS version."
            FORCE)
    endif ()

else ()
    env_log(Cached installed NodeJS version is:
            \"${ENV_BIND_NODE_INSTALLED_VERSION}\".)
endif ()


# NodeJS checksum version -----------------------------------------------------

if (NOT ENV_BIND_NODE_CHECKSUMS_VERSION)
    if (ENV_BIND_NODE_VERSION STREQUAL installed)
        if (ENV_BIND_NODE_INSTALLED)
            set(__env_bind_node_checksums_version
                "${ENV_BIND_NODE_INSTALLED_VERSION}")
        else ()
            set(__env_bind_node_checksums_version
                "${ENV_BIND_NODE_FALLBACK_VERSION}")
        endif ()
    else ()
        set(__env_bind_node_checksums_version
            "${ENV_BIND_NODE_VERSION}")
    endif ()

    set(ENV_BIND_NODE_CHECKSUMS_VERSION
        "${__env_bind_node_checksums_version}"
        CACHE STRING
        "Version for NodeJS bindings checksums."
        FORCE)
endif ()

env_log(NodeJS version used for checksums download:
        \"${ENV_BIND_NODE_CHECKSUMS_VERSION}\".)


# NodeJS checksum file --------------------------------------------------------

if (NOT ENV_BIND_NODE_CHECKSUMS_FILE)
    string(JOIN / __env_bind_node_checksums_url
           ${ENV_BIND_NODE_ROOT_DOWNLOAD_URL}
           ${ENV_BIND_NODE_CHECKSUMS_VERSION}
           ${ENV_BIND_NODE_CHECKSUMS})

    set(ENV_BIND_NODE_CHECKSUMS_URL
        "${__env_bind_node_checksums_url}"
        CACHE STRING
        "Url to NodeJS header and library archives' checksums file."
        FORCE)

    env_fetch(checksums
              URL ${ENV_BIND_NODE_CHECKSUMS_URL}
              DOWNLOAD_NO_EXTRACT TRUE
              JUST_FETCH)

    string(JOIN / __env_bind_node_checksums_file
           "${env_bind_node_checksums_src_dir}"
           "${ENV_BIND_NODE_CHECKSUMS}")

    set(ENV_BIND_NODE_CHECKSUMS_FILE
        "${__env_bind_node_checksums_file}"
        CACHE STRING
        "Path to NodeJS headers and library archives' checksums file.")
endif ()

env_log(NodeJS header and library archive checksums file fetched in:
        \"${ENV_BIND_NODE_CHECKSUM_FILE}\".)


# Headers' components ---------------------------------------------------------

# Extract NodeJS name, archive version, headers checksum, and headers archive
# from checksums file.

if (NOT ENV_BIND_NODE_NAME OR
    NOT ENV_BIND_NODE_ARCHIVE_VERSION OR

    NOT ENV_BIND_NODE_HEADERS_ARCHIVE_CHECKSUM OR
    NOT ENV_BIND_NODE_REMOTE_HEADERS_ARCHIVE_RPATH OR

    NOT ENV_BIND_NODE_HEADERS_ARCHIVE_URL OR

    NOT ENV_BIND_NODE_ID)

    set(__env_bind_node_checksums_headers_regex
        [[^([A-Fa-f0-9]+)[\t ]+([^-]+)-(headers|v?[0-9.]+)-(headers|v?[0-9.]+)([.]tar[.]gz)$]]
        CACHE STRING
        "Regex for matching NodeJS components from checksums file."
        FORCE)

    file(STRINGS
         ${ENV_BIND_NODE_CHECKSUMS_FILE}
         __env_bind_node_headers_checksum_full
         REGEX ${__env_bind_node_checksums_headers_regex}
         LIMIT_COUNT 1)

    if (NOT __env_bind_node_headers_checksum_full)
        env_log(FATAL_ERROR Unable to extract NodeJS header archive checksum.)
    endif ()

    string(REGEX MATCH
           ${__env_bind_node_checksums_headers_regex}
           __env_bind_node_headers_checksum_match
           ${__env_bind_node_headers_checksum_full})


    set(ENV_BIND_NODE_NAME
        "${CMAKE_MATCH_2}"
        CACHE STRING
        "NodeJS name.")

    if (CMAKE_MATCH_3 STREQUAL headers)
        set(__env_bind_node_archive_version ${CMAKE_MATCH_4})
    else ()
        set(__env_bind_node_archive_version ${CMAKE_MATCH_3})
    endif ()

    set(ENV_BIND_NODE_ARCHIVE_VERSION
        "${__env_bind_node_archive_version}"
        CACHE STRING
        "NodeJS version used for download of headers' and library archives.")

    set(ENV_BIND_NODE_ID
        "${ENV_BIND_NODE_NAME}-${ENV_BIND_NODE_ARCHIVE_VERSION}"
        CACHE STRING
        "Identical to NodeJS include folder in NodeJS header's archive ")


    set(ENV_BIND_NODE_HEADERS_ARCHIVE_CHECKSUM
        "${CMAKE_MATCH_1}"
        CACHE STRING
        "NodeJS headers checksum.")

    set(ENV_BIND_NODE_REMOTE_HEADERS_ARCHIVE_RPATH
        "${ENV_BIND_NODE_NAME}-${CMAKE_MATCH_3}-${CMAKE_MATCH_4}${CMAKE_MATCH_5}"
        CACHE STRING
        "Remote NodeJS headers archive path relative to version folder.")

    string(JOIN / __env_bind_node_headers_archive_url
           "${ENV_BIND_NODE_ROOT_DOWNLOAD_URL}"
           "${ENV_BIND_NODE_ARCHIVE_VERSION}"
           "${ENV_BIND_NODE_REMOTE_HEADERS_ARCHIVE_RPATH}")

    set(ENV_BIND_NODE_HEADERS_ARCHIVE_URL
        "${__env_bind_node_headers_archive_url}"
        CACHE STRING
        "Url from which the NodeJS headers archive will be downloaded.")
endif ()

env_log(NodeJS ID used for headers' archive: \"${ENV_BIND_NODE_ID}\")


# Headers' target -------------------------------------------------------------

if (NOT TARGET env_bind_node_headers)
    set(__env_bind_node_fetch_url_hash
        "${ENV_BIND_NODE_CHECK_TYPE}=${ENV_BIND_NODE_HEADERS_CHECKSUM}")

    env_fetch(headers
              URL "${ENV_BIND_NODE_HEADERS_ARCHIVE_URL}"
              URL_HASH "${__env_bind_node_fetch_url_hash}"

              INCLUDE_DIRS
              "/${ENV_BIND_NODE_ID}/include")
endif ()

target_link_libraries(env_bind_node_node INTERFACE env_bind_node_headers)


# OS specific -----------------------------------------------------------------

if (WIN32)
    if (NOT TARGET env_bind_node_win_lib)
        env_add_export(win_lib)


        if (NOT TARGET env_bind_node_win_delay_load_hook)
            env_log(Adding Windows delay load hook.)

            add_library(
                    env_bind_node_win_delay_load_hook OBJECT
                    "${CMAKE_CURRENT_SOURCE_DIR}/src/win_delay_load_hook.cpp")

            env_target_link(win_lib INTERFACE env_bind_node_win_delay_load_hook)
        endif ()


        env_target_link_with(
                win_lib
                INTERFACE
                "-IGNORE:4199"
                "-DELAYLOAD:iojs.exe"
                "-DELAYLOAD:node.exe"
                "-DELAYLOAD:node.dll")

        env_target_link(
                win_lib
                INTERFACE
                "kernel32.lib"
                "user32.lib"
                "gdi32.lib"
                "winspool.lib"
                "comdlg32.lib"
                "advapi32.lib"
                "shell32.lib"
                "ole32.lib"
                "oleaut32.lib"
                "uuid.lib"
                "odbc32.lib"
                "Shlwapi.lib"
                "DelayImp.lib")


        if (ENV_32BIT)
            set(__env_bind_node_checksums_win_lib32_regex
                [[(^[0-9A-Fa-f]+)[\t ]+(win-x86)?(/)?([^/]*)(.lib)]]
                CACHE STRING
                "Regex for matching NodeJS Windows 32 bit library components from its name."
                FORCE)

            file(STRINGS
                 ${ENV_BIND_NODE_CHECKSUMS_FILE}
                 __env_bind_node_win_lib32_checksum_full
                 REGEX ${__env_bind_node_win_lib32_regex}
                 LIMIT_COUNT 1)

            if (NOT __env_bind_node_win_lib32_checksum_full)
                env_log(FATAL_ERROR Unable to extract
                        NodeJS Windows 32 bit library checksum.)
            endif ()

            string(REGEX MATCH
                   ${__env_bind_node_win_lib32_regex}
                   __env_bind_node_win_lib32_checksum_match
                   ${__env_bind_node_win_lib32_checksum_full})


            set(ENV_BIND_NODE_WIN_LIB32_CHECKSUM
                ${CMAKE_MATCH_1}
                CACHE STRING
                "Checksum for the NodeJS Windows 32 bit library.")

            string(JOIN "" __env_bind_node_remote_win_lib32_rpath
                   "${CMAKE_MATCH_2}"
                   "${CMAKE_MATCH_3}"
                   "${CMAKE_MATCH_4}"
                   "${CMAKE_MATCH_5}")

            set(ENV_BIND_NODE_REMOTE_WIN_LIB32_RPATH
                "${__env_bind_node_remote_win_lib32_rpath}"
                CACHE STRING
                "\
NodeJS Windows 32 bit library path relative to the remote \
archive version folder.")

            string(JOIN / __env_bind_node_win_lib32_url
                   "${ENV_BIND_NODE_ROOT_DOWNLOAD_URL}"
                   "${ENV_BIND_NODE_ARCHIVE_VERSION}"
                   "${ENV_BIND_NODE_REMOTE_WIN_LIB32_RPATH}")

            set(ENV_BIND_NODE_WIN_LIB32_URL
                "${__env_bind_node_win_lib32_url}"
                CACHE STRING
                "Url to download the NodeJS Windows 32 bit library from.")

            string(JOIN = __env_bind_node_fetch_url_hash
                   "${ENV_BIND_NODE_CHECK_TYPE}"
                   "${ENV_BIND_NODE_WIN_LIB32_CHECKSUM}")

            env_fetch(win_lib32
                      URL "${ENV_BIND_NODE_WIN_LIB32_URL}"
                      URL_HASH "${__env_bind_node_fetch_url_hash}"
                      DOWNLOAD_NO_EXTRACT TRUE

                      LIB_GLOB "/*.lib")

            env_target_link(win_lib INTERFACE env_bind_node::win_lib32)


        elseif (ENV_64BIT)
            set(__env_bind_node_checksums_lib64_regex
                [[(^[0-9A-Fa-f]+)[\t ]+(win-)?(x64/)(.*)(.lib)$]]
                CACHE STRING
                "Regex for matching Windows NodeJS 64 bit library components from its name."
                FORCE)

            file(STRINGS
                 ${ENV_BIND_NODE_CHECKSUMS_FILE}
                 __env_bind_node_win_lib64_checksum_full
                 REGEX ${__env_bind_node_win_lib64_regex}
                 LIMIT_COUNT 1)

            if (NOT __env_bind_node_win_lib64_checksum_full)
                env_log(FATAL_ERROR Unable to extract
                        NodeJS Windows 32 bit library checksum.)
            endif ()

            string(REGEX MATCH
                   ${__env_bind_node_win_lib64_regex}
                   __env_bind_node_win_lib64_checksum_match
                   ${__env_bind_node_win_lib64_checksum_full})


            set(ENV_BIND_NODE_WIN_LIB64_CHECKSUM
                ${CMAKE_MATCH_1}
                CACHE STRING
                "Checksum for the NodeJS Windows 32 bit library.")

            string(JOIN "" __env_bind_node_remote_win_lib64_rpath
                   "${CMAKE_MATCH_2}"
                   "${CMAKE_MATCH_3}"
                   "${CMAKE_MATCH_4}"
                   "${CMAKE_MATCH_5}")

            set(ENV_BIND_NODE_REMOTE_WIN_LIB64_RPATH
                "${__env_bind_node_remote_win_lib64_rpath}"
                CACHE STRING
                "\
NodeJS Windows 32 bit library path relative to the remote \
archive version folder.")

            string(JOIN / __env_bind_node_win_lib64_url
                   "${ENV_BIND_NODE_ROOT_DOWNLOAD_URL}"
                   "${ENV_BIND_NODE_ARCHIVE_VERSION}"
                   "${ENV_BIND_NODE_REMOTE_WIN_LIB64_RPATH}")

            set(ENV_BIND_NODE_WIN_LIB64_URL
                "${__env_bind_node_win_lib64_url}"
                CACHE STRING
                "Url to download the NodeJS Windows 32 bit library from.")

            string(JOIN = __env_bind_node_fetch_url_hash
                   "${ENV_BIND_NODE_CHECK_TYPE}"
                   "${ENV_BIND_NODE_WIN_LIB64_CHECKSUM}")

            env_fetch(win_lib64
                      URL "${ENV_BIND_NODE_WIN_LIB64_URL}"
                      URL_HASH "${__env_bind_node_fetch_url_hash}"
                      DOWNLOAD_NO_EXTRACT TRUE

                      LIB_GLOB "/*.lib")

            env_target_link(win_lib INTERFACE env_bind_node::win_lib64)
        endif ()
    endif ()

    env_target_link(node INTERFACE env_bind_node::win_lib)

else ()
    env_target_definitions(
            node
            INTERFACE
            "_LARGEFILE_SOURCE"
            "_FILE_OFFSET_BITS=64")
endif ()


if (APPLE)
    env_target_link_with(node INTERFACE "-undefined dynamic_lookup")
    env_target_definitions(node INTERFACE "_DARWIN_USE_64_BIT_INODE=1")
endif ()


# Abstractions ----------------------------------------------------------------

if (NOT TARGET env_bind_node::abstractions)
    if (ENV_BIND_NODE_USE_NAN)
        env_fetch(abstractions
                  GIT_REPOSITORY https://github.com/nodejs/nan.git
                  GIT_TAG v2.14.2

                  INCLUDE_DIRS "/")
    else ()
        env_fetch(abstractions
                  GIT_REPOSITORY https://github.com/nodejs/node-addon-api.git
                  GIT_TAG 3.1.0

                  INCLUDE_DIRS "/")
    endif ()
endif ()

env_target_link(node INTERFACE env_bind_node::abstractions)


# Add binding -----------------------------------------------------------------

function(env_bind_node_add _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding NodeJS binding \"${_name}\".)

    string(MAKE_C_IDENTIFIER ${_mod} _sym_check)
    if (NOT "${_mod}" STREQUAL "${_sym_check}")
        env_log(FATAL_ERROR
                NodeJS binding name name must be a valid C identifier.
                Suggested alternative: \"${_sym_check}\".)
    endif ()

    env_add_shared(${_mod} SHARED ${ARGN})
    env_target_link(${_mod} PRIVATE env_bind_node::node)
    env_target_set(${_mod} PREFIX "" SUFFIX ".node" MACOSX_RPATH ON)

    string(TOUPPER "${_mod}_BUILD" _build_def)
    env_macro_name(${_mod} _prefix)
    env_use_upper_project_name()
    env_target_definitions(
            ${_mod}
            PRIVATE

            "MODULE__name=${_mod}"
            "${_build_def}"

            "${_prefix}_NODE_BIND=1"
            "${UPPER_PROJECT_NAME}_NODE_BIND=1"
            "ENV_NODE_BIND=1")
endfunction()
