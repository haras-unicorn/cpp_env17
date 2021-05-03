include_guard()


# -----------------------------------------------------------------------------
# Project
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.17...3.20)

project(env_node
        DESCRIPTION "Environment subproject for creating NodeJS bindings."
        VERSION 1.0
        LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(EnvFunc)
include(EnvNodeUtils)

env_project_initialize()


# Options ---------------------------------------------------------------------

set(ENV_NODE_REMOTE_ROOT_URL
    [[https://nodejs.org/download/release]]
    CACHE STRING
    "Root url for downloading NodeJS headers and libraries.")


set(ENV_NODE_VERSION
    "installed"
    CACHE STRING
    "\
Version of NodeJS headers and libraries to download and link against \
bindings.")

set(ENV_NODE_FALLBACK_VERSION
    "latest"
    CACHE STRING
    "\
Version of NodeJS to download when version is set to \"installed\" and \
NodeJS is not installed or it cannot be queried for its version.")


set(ENV_NODE_CHECK_TYPE
    "SHA256"
    CACHE STRING
    "Check type to use for NodeJS headers' archive and library checksums.")

set(ENV_NODE_REMOTE_CHECKSUMS_FILE
    "SHASUMS256.txt"
    CACHE STRING
    "Name of remote NodeJS checksums file.")

set(ENV_NODE_REMOTE_CHECKSUMS_RPATH
    ""
    CACHE STRING
    "\
Path to checksums of NodeJS headers' archive and library relative to \
remote NodeJS root url.")

set(ENV_NODE_CHECKSUMS_URL
    ""
    CACHE STRING
    "\
Full Url to NodeJS checksums file. If specified the \
ENV_NODE_REMOTE_CHECKSUMS_RPATH option is ignored.")

set(ENV_NODE_CHECKSUMS_PATH
    ""
    CACHE STRING
    "\
File to use for NodeJS headers' archive and library checksums. If set \
ENV_NODE_REMOTE_CHECKSUMS_URL is ignored because there is no need \
to download a checksums file.")


set(ENV_NODE_HEADERS_ARCHIVE_CHECKSUM
    ""
    CACHE STRING
    "\
Checksum to use for checking the download of \
NodeJS headers' archive.")

set(ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH
    ""
    CACHE STRING
    "\
Path to NodeJS headers' archive relative to \
remote NodeJS root url.")

set(ENV_NODE_HEADERS_ARCHIVE_URL
    ""
    CACHE STRING
    "\
Full Url to NodeJS headers' archive. If specified the \
ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH option is ignored.")

set(ENV_NODE_HEADERS_INCLUDE_PATH
    ""
    CACHE STRING
    "\
Path to NodeJS headers' include path. If specified \
ENV_NODE_HEADERS_ARCHIVE_URL and ENV_NODE_HEADERS_ARCHIVE_CHECKSUM \
options are ignored because there is no need to download the headers' \
archive.")


set(ENV_NODE_WIN_LIB32_CHECKSUM
    ""
    CACHE STRING
    "\
Checksum to use for checking the download of \
NodeJS Windows 32 bit library.")

set(ENV_NODE_REMOTE_WIN_LIB32_RPATH
    ""
    CACHE STRING
    "\
Path to NodeJS Windows 32 bit library relative to \
remote NodeJS root url.")

set(ENV_NODE_WIN_LIB32_URL
    ""
    CACHE STRING
    "\
Full Url to NodeJS Windows 32 bit library. If specified the \
ENV_NODE_REMOTE_WIN_LIB32_RPATH option is ignored.")

set(ENV_NODE_WIN_LIB32_PATH
    ""
    CACHE STRING
    "\
Path to NodeJS Windows 32 bit library. If specified the \
ENV_NODE_WIN_LIB32_URL and ENV_NODE_WIN_LIB32_CHECKSUM options are ignored \
because there is no need to download the NodeJS Windows 32 bit library.")


set(ENV_NODE_WIN_LIB64_CHECKSUM
    ""
    CACHE STRING
    "\
Checksum to use for checking the download of \
NodeJS Windows 64 bit library.")

set(ENV_NODE_REMOTE_WIN_LIB64_RPATH
    ""
    CACHE STRING
    "\
Path to NodeJS Windows 64 bit library relative to \
remote NodeJS root url.")

set(ENV_NODE_WIN_LIB64_URL
    ""
    CACHE STRING
    "\
Full Url to NodeJS Windows 64 bit library. If specified the \
ENV_NODE_REMOTE_WIN_LIB64_RPATH option is ignored.")

set(ENV_NODE_WIN_LIB64_PATH
    ""
    CACHE STRING
    "\
Path to NodeJS Windows 64 bit library. If specified the \
ENV_NODE_WIN_LIB64_URL and ENV_NODE_WIN_LIB64_CHECKSUM options are ignored \
because there is no need to download the NodeJS Windows 64 bit library.")


option(ENV_NODE_USE_NAN
       "\
Use Native Abstractions for NodeJS (https://github.com/nodejs/nan)
instead of node-addon-api (https://github.com/nodejs/node-addon-api) for \
abstraction over NodeJS versions. Not recommended.")

if (ENV_NODE_USE_NAN)
    set(ENV_NODE_ABSTRACTIONS_TAG
        "v2.14.2"
        CACHE STRING
        "Github tag to use to download NodeJS abstractions.")
else ()
    set(ENV_NODE_ABSTRACTIONS_TAG
        "3.1.0"
        CACHE STRING
        "Github tag to use to download NodeJS abstractions.")
endif ()


# -----------------------------------------------------------------------------
# Versioning and checksums
# -----------------------------------------------------------------------------

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
            "${ENV_NODE_FALLBACK_VERSION}"
            __env_node_checksums_version)

    set(ENV_NODE_CHECKSUMS_VERSION
        "${__env_node_checksums_version}"
        CACHE STRING
        "Version for NodeJS bindings checksums."
        FORCE)

    env_set_if(ENV_NODE_REMOTE_CHECKSUMS_RPATH
               "${ENV_NODE_CHECKSUMS_VERSION}/${ENV_NODE_REMOTE_CHECKSUMS_FILE}"
               CACHE STRING
               "\
Path to checksums of NodeJS headers' archive and library relative to \
remote NodeJS root url."
               FORCE)

    env_set_if(ENV_NODE_CHECKSUMS_URL
               "${ENV_NODE_REMOTE_ROOT_URL}/${ENV_NODE_REMOTE_CHECKSUMS_RPATH}"
               CACHE STRING
               "\
Full Url to NodeJS checksums file. If specified the \
ENV_NODE_REMOTE_CHECKSUMS_RPATH option is ignored."
               FORCE)

    env_log(NodeJS checksums url is
            \"${ENV_NODE_CHECKSUMS_URL}\".)

else ()
    env_log(Cached NodeJS checksums version is
            \"${ENV_NODE_CHECKSUMS_VERSION}\".)

    env_log(Cached NodeJS checksums url is
            \"${ENV_NODE_CHECKSUMS_URL}\".)
endif ()


# NodeJS checksums file -------------------------------------------------------

if (NOT ENV_NODE_CHECKSUMS_PATH)
    env_node_fetch_checksums_file(
            "${ENV_NODE_CHECKSUMS_URL}"
            "${ENV_NODE_REMOTE_CHECKSUMS_FILE}"
            __env_node_checksums_path)

    env_set_if(ENV_NODE_CHECKSUMS_PATH
               "${__env_node_checksums_path}"
               CACHE STRING
               "\
File to use for NodeJS headers' archive and library checksums. If set \
ENV_NODE_REMOTE_CHECKSUMS_RPATH is ignored because there is no need \
to download a checksums file."
               FORCE)

else ()
    env_log(Cached NodeJS checksums path
            \"${ENV_NODE_CHECKSUMS_PATH}\".)
endif ()


# -----------------------------------------------------------------------------
# Checksums file components
# -----------------------------------------------------------------------------

# Headers' components ---------------------------------------------------------

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
            __env_node_headers_archive_checksum
            __env_node_headers_archive_rpath)


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


    env_set_if(ENV_NODE_HEADERS_ARCHIVE_CHECKSUM
               "${__env_node_headers_archive_checksum}"
               CACHE STRING
               "\
Checksum to use for checking the download of \
NodeJS headers' archive."
               FORCE)

    env_set_if(ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH
               "${__env_node_headers_archive_rpath}"
               CACHE STRING
               "\
Path to NodeJS headers' archive relative to \
remote NodeJS root url."
               FORCE)

    env_set_if(ENV_NODE_HEADERS_ARCHIVE_URL
               "${ENV_NODE_REMOTE_ROOT_URL}/${ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH}"
               CACHE STRING
               "\
Full Url to NodeJS headers' archive. If specified the \
ENV_NODE_REMOTE_HEADERS_ARCHIVE_RPATH option is ignored."
               FORCE)


else ()
    env_log(Cached NodeJS ID used for headers' archive is
            \"${ENV_NODE_ID}\")

    env_log(Cached NodeJS headers' archive checksum is
            \"${ENV_NODE_HEADERS_ARCHIVE_CHECKSUM}\".)

    env_log(Cached NodeJS headers' archive url is
            \"${ENV_NODE_HEADERS_ARCHIVE_URL}\".)
endif ()


# Windows 32 bit library components -------------------------------------------

if (NOT ENV_NODE_WIN_LIB32_CHECKSUM OR
    NOT ENV_NODE_REMOTE_WIN_LIB32_RPATH OR

    NOT ENV_NODE_WIN_LIB32_URL)

    env_node_extract_win_lib32_components(
            "${ENV_NODE_CHECKSUMS_PATH}"
            __env_node_win_lib32_checksum
            __env_node_win_lib32_version_rpath)


    env_set_if(ENV_NODE_WIN_LIB32_CHECKSUM
               "${__env_node_win_lib32_checksum}"
               CACHE STRING
               "\
Checksum to use for checking the download of \
NodeJS Windows 32 bit library."
               FORCE)

    env_set_if(ENV_NODE_REMOTE_WIN_LIB32_RPATH
               "${ENV_NODE_TARGET_VERSION}/${__env_node_win_lib32_version_rpath}"
               CACHE STRING
               "\
Path to NodeJS Windows 32 bit library relative to \
remote NodeJS root url."
               FORCE)

    env_set_if(ENV_NODE_WIN_LIB32_URL
               "${ENV_NODE_REMOTE_ROOT_URL}/${ENV_NODE_REMOTE_WIN_LIB32_RPATH}"
               CACHE STRING
               "\
Full Url to NodeJS Windows 32 bit library. If specified the \
ENV_NODE_REMOTE_WIN_LIB32_RPATH option is ignored."
               FORCE)


else ()
    env_log(Cached NodeJS Windows 32 bit library checksum is
            \"${ENV_NODE_WIN_LIB32_CHECKSUM}\".)

    env_log(Cached NodeJS Windows 32 bit library url is
            \"${ENV_NODE_WIN_LIB32_URL}\".)
endif ()


# Windows 64 bit library components -------------------------------------------

if (NOT ENV_NODE_WIN_LIB64_CHECKSUM OR
    NOT ENV_NODE_REMOTE_WIN_LIB64_RPATH OR

    NOT ENV_NODE_WIN_LIB64_URL)

    env_node_extract_win_lib64_components(
            "${ENV_NODE_CHECKSUMS_PATH}"
            __env_node_win_lib64_checksum
            __env_node_win_lib64_version_rpath)


    env_set_if(ENV_NODE_WIN_LIB64_CHECKSUM
               "${__env_node_win_lib64_checksum}"
               CACHE STRING
               "\
Checksum to use for checking the download of \
NodeJS Windows 64 bit library."
               FORCE)

    env_set_if(ENV_NODE_REMOTE_WIN_LIB64_RPATH
               "${ENV_NODE_TARGET_VERSION}/${__env_node_win_lib64_version_rpath}"
               CACHE STRING
               "\
Path to NodeJS Windows 64 bit library relative to \
remote NodeJS root url."
               FORCE)

    env_set_if(ENV_NODE_WIN_LIB64_URL
               "${ENV_NODE_REMOTE_ROOT_URL}/${ENV_NODE_REMOTE_WIN_LIB64_RPATH}"
               CACHE STRING
               "\
Full Url to NodeJS Windows 64 bit library. If specified the \
ENV_NODE_REMOTE_WIN_LIB64_RPATH option is ignored."
               FORCE)


else ()
    env_log(Cached NodeJS Windows 64 bit library checksum is
            \"${ENV_NODE_WIN_LIB64_CHECKSUM}\".)

    env_log(Cached NodeJS Windows 64 bit library url is
            \"${ENV_NODE_WIN_LIB64_URL}\".)
endif ()


# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------

# Headers ---------------------------------------------------------------------

if (NOT TARGET env_node::headers)
    if (ENV_NODE_HEADERS_INCLUDE_PATH)
        env_add_export(headers)
        env_target_include(
                headers
                INTERFACE
                "${ENV_NODE_HEADERS_INCLUDE_PATH}")

    else ()
        env_node_fetch_headers(
                "${ENV_NODE_HEADERS_ARCHIVE_URL}"
                "${ENV_NODE_CHECK_TYPE}"
                "${ENV_NODE_HEADERS_ARCHIVE_CHECKSUM}")

    endif ()
endif ()


# OS specific -----------------------------------------------------------------

if (NOT TARGET env_node::os)
    if (ENV_WIN AND NOT TARGET env_node::win_lib)
        if (ENV_32BIT)
            if (ENV_NODE_WIN_LIB32_PATH)
                env_add_export(win_lib "${ENV_NODE_WIN_LIB32_PATH}")
            else ()
                env_node_fetch_win_lib(
                        "${ENV_NODE_WIN_LIB32_URL}"
                        "${ENV_NODE_CHECK_TYPE}"
                        "${ENV_NODE_WIN_LIB32_CHECKSUM}")
            endif ()
        elseif (ENV_64BIT)
            if (ENV_NODE_WIN_LIB64_PATH)
                env_add_export(win_lib "${ENV_NODE_WIN_LIB64_PATH}")
            else ()
                env_node_fetch_win_lib(
                        "${ENV_NODE_WIN_LIB64_URL}"
                        "${ENV_NODE_CHECK_TYPE}"
                        "${ENV_NODE_WIN_LIB64_CHECKSUM}")
            endif ()
        endif ()
    endif ()

    env_node_add_os_specific_interface(
            "${PROJECT_SOURCE_DIR}/assets/win_delay_load_hook.cpp"
            env_node::win_lib)
endif ()


# Abstractions ----------------------------------------------------------------

if (NOT TARGET env_node::abstractions)
    if (ENV_NODE_USE_NAN)
        env_log(Using Native Abstractions for NodeJS for NodeJS abstractions.)
        env_log(Git tag for Native Abstractions for NodeJS is
                \"${ENV_NODE_ABSTRACTIONS_TAG}\".)

        env_fetch(abstractions
                  FROM
                  GIT_REPOSITORY https://github.com/nodejs/nan.git
                  GIT_TAG "${ENV_NODE_ABSTRACTIONS_TAG}"

                  INTO node

                  SCAFFOLD
                  INCLUDE_DIRS "/")
    else ()
        env_log(Using node-addon-api for NodeJS abstractions.)
        env_log(Git tag for node-addon-api is
                \"${ENV_NODE_ABSTRACTIONS_TAG}\".)

        env_fetch(abstractions
                  FROM
                  GIT_REPOSITORY https://github.com/nodejs/node-addon-api.git
                  GIT_TAG "${ENV_NODE_ABSTRACTIONS_TAG}"

                  INTO node

                  SCAFFOLD
                  INCLUDE_DIRS "/")
    endif ()
endif ()


# -----------------------------------------------------------------------------
# Targets
# -----------------------------------------------------------------------------

env_project_targets(
        DEPS
        env::export
        env_node::headers
        env_node::os
        env_node::abstractions

        TEST_DEPS
        env::test

        BENCH_DEPS
        env::bench)


# Add binding -----------------------------------------------------------------

function(env_node_add_bind _name)
    env_log(Adding NodeJS binding \"${_name}\".)

    env_add_module(${_name} ${ARGN})
    env_target_link(${_name} PRIVATE env_node::export)

    env_node_decorate_bind(${_name})
endfunction()
