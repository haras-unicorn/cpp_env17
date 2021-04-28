include_guard()


# Project ---------------------------------------------------------------------

cmake_minimum_required(VERSION 3.17...3.20)

project(env_node
        DESCRIPTION "Environment subproject for creating NodeJS bindings."
        VERSION 1.0
        LANGUAGES C CXX)

include("${env_SOURCE_DIR}/cmake/EnvFunc.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/EnvNodeUtils.cmake")


# Options ---------------------------------------------------------------------

option(ENV_NODE_REMOTE_ROOT_URL
       "Root url for downloading NodeJS headers and libraries."
       [[https://nodejs.org/download/release]])


option(ENV_NODE_VERSION
       "\
Version of NodeJS headers and libraries to download and link against \
bindings."
       "installed")

option(ENV_NODE_FALLBACK_VERSION
       "\
Version of NodeJS to download when version is set to \"installed\" and \
NodeJS is not installed or it cannot be queried for its version."
       "latest")


option(ENV_NODE_CHECK_TYPE
       "Check type to use for NodeJS headers' archive and library checksums."
       "SHA256")

option(ENV_NODE_REMOTE_CHECKSUMS_FILE
       "Name of remote NodeJS checksums file."
       "SHASUMS256.txt")

option(ENV_NODE_REMOTE_CHECKSUMS_RPATH
       "\
Path to checksums of NodeJS headers' archive and library relative to \
remote NodeJS root url."
       "")

option(ENV_NODE_CHECKSUMS_URL
       "\
Full Url to NodeJS checksums file. If specified the \
ENV_NODE_REMOTE_CHECKSUMS_RPATH option is ignored."
       "")

option(ENV_NODE_CHECKSUMS_PATH
       "\
File to use for NodeJS headers' archive and library checksums. If set \
ENV_NODE_REMOTE_CHECKSUMS_URL is ignored because there is no need \
to download a checksums file."
       "")


option(ENV_NODE_HEADERS_ARCHIVE_CHECKSUM
       "\
Checksum to use for checking the download of \
NodeJS headers' archive."
       "")

option(ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH
       "\
Path to NodeJS headers' archive relative to \
remote NodeJS root url."
       "")

option(ENV_NODE_HEADERS_ARCHIVE_URL
       "\
Full Url to NodeJS headers' archive. If specified the \
ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH option is ignored."
       "")


option(ENV_NODE_WIN_LIB32_CHECKSUM
       "\
Checksum to use for checking the download of \
NodeJS Windows 32 bit library."
       "")

option(ENV_NODE_REMOTE_WIN_LIB32_RPATH
       "\
Path to NodeJS Windows 32 bit library relative to \
remote NodeJS root url."
       "")

option(ENV_NODE_WIN_LIB32_URL
       "\
Full Url to NodeJS Windows 32 bit library. If specified the \
ENV_NODE_REMOTE_WIN_LIB32_RPATH option is ignored."
       "")


option(ENV_NODE_WIN_LIB64_CHECKSUM
       "\
Checksum to use for checking the download of \
NodeJS Windows 64 bit library."
       "")

option(ENV_NODE_REMOTE_WIN_LIB64_RPATH
       "\
Path to NodeJS Windows 64 bit library relative to \
remote NodeJS root url."
       "")

option(ENV_NODE_WIN_LIB64_URL
       "\
Full Url to NodeJS Windows 64 bit library. If specified the \
ENV_NODE_REMOTE_WIN_LIB64_RPATH option is ignored."
       "")


option(ENV_NODE_USE_NAN
       "\
Use Native Abstractions for NodeJS instead of node-addon-api for \
abstraction over NodeJS versions. Not recommended."
       OFF)


# Dependencies ----------------------------------------------------------------

# Node dependency -------------------------------------------------------------

if (NOT TARGET env_node::node)
    env_add_export(node)
endif ()


# Installed NodeJS version ----------------------------------------------------

if (NOT ENV_NODE_INSTALLED_VERSION)
    env_node_detect_installed_version(
            __env_node_installed_version)

    set(ENV_NODE_INSTALLED_VERSION
        "${__env_node_installed_version}"
        CACHE STRING
        "Installed NodeJS version."
        FORCE)
else ()
    env_log(Cached installed NodeJS version is
            \"${ENV_NODE_INSTALLED_VERSION}\".)
endif ()


# NodeJS checksum version -----------------------------------------------------

if (NOT ENV_NODE_CHECKSUMS_VERSION OR
    NOT ENV_NODE_REMOTE_CHECKSUMS_RPATH OR
    NOT ENV_NODE_CHECKSUMS_URL)
    env_node_resolve_checksums_version(
            "${ENV_NODE_VERSION}"
            "${ENV_NODE_INSTALLED_VERSION}"
            __env_node_checksums_version)

    set(ENV_NODE_CHECKSUMS_VERSION
        "${__env_node_checksums_version}"
        CACHE STRING
        "Version for NodeJS bindings checksums."
        FORCE)

    set(ENV_NODE_REMOTE_CHECKSUMS_RPATH
        "${ENV_NODE_CHECKSUMS_VERSION}/${ENV_NODE_REMOTE_CHECKSUMS_FILE}"
        "\
Path to checksums of NodeJS headers' archive and library relative to \
remote NodeJS root url."
        CACHE STRING)

    set(ENV_NODE_CHECKSUMS_URL
        "${ENV_NODE_REMOTE_ROOT_URL}/${ENV_NODE_REMOTE_CHECKSUMS_RPATH}"
        "\
Full Url to NodeJS checksums file. If specified the \
ENV_NODE_REMOTE_CHECKSUMS_RPATH option is ignored."
        CACHE STRING)

else ()
    env_log(Cached NodeJS checksums version is:
            \"${ENV_NODE_CHECKSUMS_VERSION}\".)
endif ()


# NodeJS checksums file -------------------------------------------------------

if (NOT ENV_NODE_CHECKSUMS_PATH)
    env_node_fetch_checksums_file(
            "${ENV_NODE_CHECKSUMS_URL}"
            "${ENV_NODE_REMOTE_CHECKSUMS_FILE}"
            __env_node_checksums_path)

    set(ENV_NODE_CHECKSUMS_PATH
        "${__env_node_checksums_path}"
        CACHE STRING
        "\
File to use for NodeJS headers' archive and library checksums. If set \
ENV_NODE_REMOTE_CHECKSUMS_RPATH is ignored because there is no need \
to download a checksums file.")
else ()
    env_log(Cached NodeJS checksums file location in:
            \"${ENV_NODE_CHECKSUMS_PATH}\".)
endif ()


# Headers' components ---------------------------------------------------------

# Extract NodeJS name, archive version, headers checksum, and headers archive
# from checksums file.

if (NOT ENV_NODE_NAME OR
    NOT ENV_NODE_TARGET_VERSION OR
    NOT ENV_NODE_ID OR

    NOT ENV_NODE_HEADERS_ARCHIVE_CHECKSUM OR
    NOT ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH OR

    NOT ENV_NODE_HEADERS_ARCHIVE_URL)

    env_node_extract_headers_archive_components(
            "${ENV_NODE_CHECKSUMS_PATH}"
            __env_node_name
            __env_node_target_version
            __env_node_id
            __env_node_headers_checksum
            __env_node_headers_rpath)


    set(ENV_NODE_NAME
        "${__env_node_name}"
        CACHE STRING
        "NodeJS name."
        FORCE)

    set(ENV_NODE_TARGET_VERSION
        "${__env_node_target_version}"
        CACHE STRING
        "NodeJS version used for download of headers' archive and library."
        FORCE)

    set(ENV_NODE_ID
        "${__env_node_id}"
        CACHE STRING
        "Identical to NodeJS include folder in NodeJS header's archive."
        FORCE)


    set(ENV_NODE_HEADERS_ARCHIVE_CHECKSUM
        "${__env_node_headers_archive_checksum}"
        CACHE STRING
        "\
Checksum to use for checking the download of \
NodeJS headers' archive.")

    set(ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH
        "${__env_node_headers_archive_rpath}"
        CACHE STRING
        "\
Path to NodeJS headers' archive relative to \
remote NodeJS root url.")

    set(ENV_NODE_HEADERS_ARCHIVE_URL
        "${ENV_NODE_REMOTE_ROOT_URL}/${ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH}"
        CACHE STRING
        "\
Full Url to NodeJS headers' archive. If specified the \
ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH option is ignored.")
else ()
    env_log(Cached NodeJS ID used for headers' archive is:
            \"${ENV_NODE_ID}\")
endif ()


# Headers' dependency ---------------------------------------------------------

if (NOT TARGET env_node::headers)
    set(__env_node_fetch_url_hash
        "${ENV_NODE_CHECK_TYPE}=${ENV_NODE_HEADERS_CHECKSUM}")

    env_fetch(headers
              URL "${ENV_NODE_HEADERS_ARCHIVE_URL}"
              URL_HASH "${__env_node_fetch_url_hash}"

              SCAFFOLD
              INCLUDE_DIRS
              "/${ENV_NODE_ID}/include")
endif ()

target_link_libraries(node INTERFACE env_node::headers)


# OS specific dependency ------------------------------------------------------

if (NOT TARGET env_node::os)
    env_add_export(os)
endif ()

env_target_link(node INTERFACE env_node::os)


if (WIN32)
    if (NOT TARGET env_node::win_lib)
        env_add_export(win_lib)


        if (NOT TARGET env_node_win_delay_load_hook)
            env_log(Adding Windows delay load hook.)

            add_library(
                    env_node_win_delay_load_hook OBJECT
                    "src/win_delay_load_hook.cpp")

            env_add_alias(win_delay_load_hook)

            env_target_link(win_lib INTERFACE env_node::win_delay_load_hook)
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
            set(__env_node_checksums_win_lib32_regex
                [[(^[0-9A-Fa-f]+)[\t ]+(win-x86)?(/)?([^/]*)(.lib)]]
                CACHE STRING
                "Regex for matching NodeJS Windows 32 bit library components from its name."
                FORCE)

            file(STRINGS
                 ${ENV_NODE_CHECKSUMS_PATH}
                 __env_node_win_lib32_checksum_full
                 REGEX ${__env_node_win_lib32_regex}
                 LIMIT_COUNT 1)

            if (NOT __env_node_win_lib32_checksum_full)
                env_log(FATAL_ERROR Unable to extract
                        NodeJS Windows 32 bit library checksum.)
            endif ()

            string(REGEX MATCH
                   ${__env_node_win_lib32_regex}
                   __env_node_win_lib32_checksum_match
                   ${__env_node_win_lib32_checksum_full})


            set(ENV_NODE_WIN_LIB32_CHECKSUM
                ${CMAKE_MATCH_1}
                CACHE STRING
                "Checksum for the NodeJS Windows 32 bit library.")

            string(JOIN "" __env_node_remote_win_lib32_rpath
                   "${CMAKE_MATCH_2}"
                   "${CMAKE_MATCH_3}"
                   "${CMAKE_MATCH_4}"
                   "${CMAKE_MATCH_5}")

            set(ENV_NODE_REMOTE_WIN_LIB32_RPATH
                "${__env_node_remote_win_lib32_rpath}"
                CACHE STRING
                "\
NodeJS Windows 32 bit library path relative to the remote \
archive version folder.")

            string(JOIN . __env_node_win_lib32_url
                   "${ENV_NODE_REMOTE_ROOT_URL}"
                   "${ENV_NODE_TARGET_VERSION}"
                   "${ENV_NODE_REMOTE_WIN_LIB32_RPATH}")

            set(ENV_NODE_WIN_LIB32_URL
                "${__env_node_win_lib32_url}"
                CACHE STRING
                "Url to download the NodeJS Windows 32 bit library from.")

            string(JOIN = __env_node_fetch_url_hash
                   "${ENV_NODE_CHECK_TYPE}"
                   "${ENV_NODE_WIN_LIB32_CHECKSUM}")

            env_fetch(win_lib32
                      URL "${ENV_NODE_WIN_LIB32_URL}"
                      URL_HASH "${__env_node_fetch_url_hash}"
                      DOWNLOAD_NO_EXTRACT TRUE

                      SCAFFOLD
                      LIB_GLOB "/*.lib")

            env_target_link(win_lib INTERFACE env_node::win_lib32)


        elseif (ENV_64BIT)
            set(__env_node_checksums_lib64_regex
                [[(^[0-9A-Fa-f]+)[\t ]+(win-)?(x64/)(.*)(.lib)$]]
                CACHE STRING
                "Regex for matching Windows NodeJS 64 bit library components from its name."
                FORCE)

            file(STRINGS
                 ${ENV_NODE_CHECKSUMS_PATH}
                 __env_node_win_lib64_checksum_full
                 REGEX ${__env_node_win_lib64_regex}
                 LIMIT_COUNT 1)

            if (NOT __env_node_win_lib64_checksum_full)
                env_log(FATAL_ERROR Unable to extract
                        NodeJS Windows 32 bit library checksum.)
            endif ()

            string(REGEX MATCH
                   ${__env_node_win_lib64_regex}
                   __env_node_win_lib64_checksum_match
                   ${__env_node_win_lib64_checksum_full})


            set(ENV_NODE_WIN_LIB64_CHECKSUM
                ${CMAKE_MATCH_1}
                CACHE STRING
                "Checksum for the NodeJS Windows 32 bit library.")

            string(JOIN "" __env_node_remote_win_lib64_rpath
                   "${CMAKE_MATCH_2}"
                   "${CMAKE_MATCH_3}"
                   "${CMAKE_MATCH_4}"
                   "${CMAKE_MATCH_5}")

            set(ENV_NODE_REMOTE_WIN_LIB64_RPATH
                "${__env_node_remote_win_lib64_rpath}"
                CACHE STRING
                "\
NodeJS Windows 32 bit library path relative to the remote \
archive version folder.")

            string(JOIN . __env_node_win_lib64_url
                   "${ENV_NODE_REMOTE_ROOT_URL}"
                   "${ENV_NODE_TARGET_VERSION}"
                   "${ENV_NODE_REMOTE_WIN_LIB64_RPATH}")

            set(ENV_NODE_WIN_LIB64_URL
                "${__env_node_win_lib64_url}"
                CACHE STRING
                "Url to download the NodeJS Windows 32 bit library from.")

            string(JOIN = __env_node_fetch_url_hash
                   "${ENV_NODE_CHECK_TYPE}"
                   "${ENV_NODE_WIN_LIB64_CHECKSUM}")

            env_fetch(win_lib64
                      URL "${ENV_NODE_WIN_LIB64_URL}"
                      URL_HASH "${__env_node_fetch_url_hash}"
                      DOWNLOAD_NO_EXTRACT TRUE

                      SCAFFOLD
                      LIB_GLOB "/*.lib")

            env_target_link(win_lib INTERFACE env_node::win_lib64)
        endif ()
    endif ()

    env_target_link(os INTERFACE env_node::win_lib)

else ()
    env_target_definitions(
            os
            INTERFACE
            "_LARGEFILE_SOURCE"
            "_FILE_OFFSET_BITS=64")
endif ()


if (APPLE)
    env_target_link_with(os INTERFACE "-undefined dynamic_lookup")
    env_target_definitions(os INTERFACE "_DARWIN_USE_64_BIT_INODE=1")
endif ()


# Abstractions dependency -----------------------------------------------------

if (NOT TARGET env_node::abstractions)
    if (ENV_NODE_USE_NAN)
        env_fetch(abstractions
                  GIT_REPOSITORY https://github.com/nodejs/nan.git
                  GIT_TAG v2.14.2

                  SCAFFOLD
                  INCLUDE_DIRS ".")
    else ()
        env_fetch(abstractions
                  GIT_REPOSITORY https://github.com/nodejs/node-addon-api.git
                  GIT_TAG 3.1.0

                  SCAFFOLD
                  INCLUDE_DIRS ".")
    endif ()
endif ()

env_target_link(node INTERFACE env_node::abstractions)


# -----------------------------------------------------------------------------
# Targets
# -----------------------------------------------------------------------------

env_project_targets()


# Add binding -----------------------------------------------------------------

function(env_node_add _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding NodeJS binding \"${_name}\".)

    string(MAKE_C_IDENTIFIER ${_mod} _sym_check)
    if (NOT "${_mod}" STREQUAL "${_sym_check}")
        env_log(FATAL_ERROR
                NodeJS binding name name must be a valid C identifier.
                Suggested alternative: \"${_sym_check}\".)
    endif ()

    env_add_shared(${_mod} SHARED ${ARGN})
    env_target_link(${_mod} PRIVATE env_node::node)
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
            "ENV_NODE=1")
endfunction()
