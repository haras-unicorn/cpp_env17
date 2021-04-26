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


# Checksums file regexes ------------------------------------------------------

set(__env_bind_node_checksums_lib32_regex
    [[(^[0-9A-Fa-f]+)[\t ]+(win-x86)?(/)?([^/]*)(.lib)]]
    CACHE STRING
    "Regex for matching NodeJS 32 bit library components from its name."
    FORCE)

set(__env_bind_node_checksums_lib64_regex
    [[(^[0-9A-Fa-f]+)[\t ]+(win-)?(x64/)(.*)(.lib)$]]
    CACHE STRING
    "Regex for matching NodeJS 64 bit library components from its name."
    FORCE)


# Version resolution ----------------------------------------------------------

# Get installed NodeJS version.

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


# Resolve NodeJS checksum version.

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


# Get NodeJS checksum file.

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
              DOWNLOAD_NO_EXTRACT TRUE)

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


# Extract NodeJS name, archive version, headers checksum, and headers archive
# from checksums file, download and extract the headers archive and create
# headers target.

if (NOT ENV_BIND_NODE_NAME OR
    NOT ENV_BIND_NODE_ARCHIVE_VERSION OR

    NOT ENV_BIND_NODE_HEADERS_CHECKSUM OR
    NOT ENV_BIND_NODE_REMOTE_HEADERS_ARCHIVE OR
    NOT ENV_BIND_NODE_HEADERS_ARCHIVE_URL OR

    NOT TARGET env_node_bind_headers)

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

    env_log(Using NodeJS with
            name \"${ENV_BIND_NODE_NAME}\" and
            archive version \"${ENV_BIND_NODE_ARCHIVE_VERSION}\")


    set(ENV_BIND_NODE_HEADERS_CHECKSUM
        "${CMAKE_MATCH_1}"
        CACHE STRING
        "NodeJS headers checksum.")

    set(ENV_BIND_NODE_REMOTE_HEADERS_ARCHIVE
        "${ENV_BIND_NODE_NAME}-${CMAKE_MATCH_3}-${CMAKE_MATCH_4}${CMAKE_MATCH_5}"
        CACHE STRING
        "NodeJS headers archive to download.")


    string(JOIN / __env_bind_node_headers_archive_url
           "${ENV_BIND_NODE_ROOT_DOWNLOAD_URL}"
           "${ENV_BIND_NODE_ARCHIVE_VERSION}"
           "${ENV_BIND_NODE_REMOTE_HEADERS_ARCHIVE}")

    set(ENV_BIND_NODE_HEADERS_ARCHIVE_URL
        "${__env_bind_node_headers_archive_url}"
        CACHE STRING
        "Url from which the NodeJS headers archive will be downloaded.")

    set(__env_bind_node_fetch_url_hash
        "${ENV_BIND_NODE_CHECK_TYPE}=${ENV_BIND_NODE_HEADERS_CHECKSUM}")

    env_fetch(headers
              URL "${ENV_BIND_NODE_HEADERS_ARCHIVE_URL}"
              URL_HASH "${__env_bind_node_fetch_url_hash}")
endif ()


# Initialization --------------------------------------------------------------

# Sets up a project to build Node.js native modules
# - Downloads required dependencies and unpacks them to the build directory.
#   Internet access is required the first invocation but not after (
#   provided the download is successful)
# - Sets up several variables for building against the downloaded
#   dependencies
# - Guarded to prevent multiple executions, so a single project hierarchy
#   will only call this once

add_library(
        __env_bind_node_win_delay_load_hook OBJECT
        "${CMAKE_CURRENT_SOURCE_DIR}/src/win_delay_load_hook.cpp")

function(env_bind_node_initialize)
    # Prevents this function from executing more than once
    if (ENV_BIND_NODE_INITIALIZED)
        return()
    endif ()

    cmake_parse_arguments(
            ENV_BIND_NODE_INITIALIZE
            ""
            "URL;NAME;VERSION;CHECKSUM;CHECK_TYPE"
            ""
            ${ARGN})


    # Unless the target is special version "latest", the parameters
    # necessary to construct the root path are known
    if (NOT VERSION STREQUAL "latest")
        set(ROOT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}/${VERSION})
        # Extract checksums from the existing checksum file
        set(CHECKSUM_TARGET ${ROOT}/CHECKSUM)
    endif ()

    # If we're trying to determine the version or we haven't saved the
    # checksum file for this version, download it from the specified server
    if (VERSION STREQUAL "latest" OR
    (DEFINED ROOT AND NOT EXISTS ${ROOT}/CHECKSUM))
        if (DEFINED ROOT)
            # Clear away the old checksum in case the new one is different
            # and/or it fails to download
            file(REMOVE ${ROOT}/CHECKSUM)
        endif ()
        file(REMOVE ${TEMP}/CHECKSUM)
        _env_bind_node_download_file(
                ${URL}/${VERSION}/${CHECKSUM}
                ${TEMP}/CHECKSUM
                INACTIVITY_TIMEOUT 10
                STATUS CHECKSUM_STATUS)
        list(GET CHECKSUM_STATUS 0 CHECKSUM_STATUS)
        if (CHECKSUM_STATUS GREATER 0)
            file(REMOVE ${TEMP}/CHECKSUM)
            message(FATAL_ERROR
                    "Unable to download checksum file")
        endif ()
        # Extract checksums from the temporary file
        set(CHECKSUM_TARGET ${TEMP}/CHECKSUM)
    endif ()

    # Extract the version, name, header archive and archive checksum
    # from the file. This first extract is what defines / specifies the
    # actual version number and name.
    file(STRINGS
         ${CHECKSUM_TARGET} HEADERS_CHECKSUM
         REGEX ${__env_bind_node_headers_regex}
         LIMIT_COUNT 1)
    if (NOT HEADERS_CHECKSUM)
        file(REMOVE ${TEMP}/CHECKSUM)
        if (DEFINED ROOT)
            file(REMOVE ${ROOT}/CHECKSUM)
        endif ()
        message(FATAL_ERROR "Unable to extract header archive checksum")
    endif ()
    string(REGEX MATCH ${__env_bind_node_headers_regex} HEADERS_CHECKSUM ${HEADERS_CHECKSUM})
    set(HEADERS_CHECKSUM ${CMAKE_MATCH_1})
    set(NAME ${CMAKE_MATCH_2})
    if (CMAKE_MATCH_3 STREQUAL "headers")
        set(VERSION ${CMAKE_MATCH_4})
    else ()
        set(VERSION ${CMAKE_MATCH_3})
    endif ()
    set(HEADERS_ARCHIVE
        ${CMAKE_MATCH_2}-${CMAKE_MATCH_3}-${CMAKE_MATCH_4}${CMAKE_MATCH_5})
    # Make sure that the root directory exists, and that the checksum
    # file has been moved over from temp
    if (DEFINED ROOT)
        set(OLD_ROOT ${ROOT})
    endif ()
    set(ROOT ${CMAKE_CURRENT_BINARY_DIR}/${NAME}/${VERSION})
    if (DEFINED OLD_ROOT AND NOT ROOT STREQUAL "${OLD_ROOT}")
        file(REMOVE ${TEMP}/CHECKSUM)
        file(REMOVE ${ROOT}/CHECKSUM)
        message(FATAL_ERROR "Version/Name mismatch")
    endif ()
    file(MAKE_DIRECTORY ${ROOT})
    if (EXISTS ${TEMP}/CHECKSUM)
        file(REMOVE ${ROOT}/CHECKSUM)
        file(RENAME ${TEMP}/CHECKSUM ${ROOT}/CHECKSUM)
    endif ()

    # Now that its fully resolved, report the name and version of Node.js being
    # used
    message(STATUS "NodeJS: Using ${NAME}, version ${VERSION}")

    # Download the headers for the version being used
    # Theoretically, these could be found by searching the installed
    # system, but in practice, this can be error prone. They're provided
    # on the download servers, so just use the ones there.
    if (NOT EXISTS ${ROOT}/include)
        file(REMOVE ${TEMP}/${HEADERS_ARCHIVE})
        env_bind_node_download_file(
                ${URL}/${VERSION}/${HEADERS_ARCHIVE}
                ${TEMP}/${HEADERS_ARCHIVE}
                INACTIVITY_TIMEOUT 10
                EXPECTED_HASH ${CHECK_TYPE}=${HEADERS_CHECKSUM}
                STATUS HEADERS_STATUS)
        list(GET HEADERS_STATUS 0 HEADERS_STATUS)
        if (HEADER_STATUS GREATER 0)
            file(REMOVE ${TEMP}/${HEADERS_ARCHIVE})
            message(FATAL_ERROR "Unable to download Node.js headers")
        endif ()
        execute_process(
                COMMAND ${CMAKE_COMMAND} -E tar xfz ${TEMP}/${HEADERS_ARCHIVE}
                WORKING_DIRECTORY ${TEMP})

        # This adapts the header extraction to support a number of different
        # header archive contents in addition to the one used by the
        # default Node.js library
        unset(ENV_BIND_NODE_HEADERS_PATH CACHE)
        find_path(ENV_BIND_NODE_HEADERS_PATH
                  NAMES src include
                  PATHS
                  ${TEMP}/${NAME}-${VERSION}-headers
                  ${TEMP}/${NAME}-${VERSION}
                  ${TEMP}/${ENV_BIND_NODE_DEFAULT_NAME}-${VERSION}-headers
                  ${TEMP}/${ENV_BIND_NODE_DEFAULT_NAME}-${VERSION}
                  ${TEMP}/${ENV_BIND_NODE_DEFAULT_NAME}
                  ${TEMP}
                  NO_DEFAULT_PATH)
        if (NOT ENV_BIND_NODE_HEADERS_PATH)
            message(FATAL_ERROR "Unable to find extracted headers folder")
        endif ()

        # Move the headers into a standard location with a standard layout
        file(REMOVE ${TEMP}/${HEADERS_ARCHIVE})
        file(REMOVE_RECURSE ${ROOT}/include)
        if (EXISTS ${ENV_BIND_NODE_HEADERS_PATH}/include/node)
            file(RENAME ${ENV_BIND_NODE_HEADERS_PATH}/include/node ${ROOT}/include)
        elseif (EXISTS ${ENV_BIND_NODE_HEADERS_PATH}/src)
            file(MAKE_DIRECTORY ${ROOT}/include)
            if (NOT EXISTS ${ENV_BIND_NODE_HEADERS_PATH}/src)
                file(REMOVE_RECURSE ${ROOT}/include)
                message(FATAL_ERROR "Unable to find core headers")
            endif ()
            file(COPY ${ENV_BIND_NODE_HEADERS_PATH}/src/
                 DESTINATION ${ROOT}/include)
            if (NOT EXISTS ${ENV_BIND_NODE_HEADERS_PATH}/deps/uv/include)
                file(REMOVE_RECURSE ${ROOT}/include)
                message(FATAL_ERROR "Unable to find libuv headers")
            endif ()
            file(COPY ${ENV_BIND_NODE_HEADERS_PATH}/deps/uv/include/
                 DESTINATION ${ROOT}/include)
            if (NOT EXISTS ${ENV_BIND_NODE_HEADERS_PATH}/deps/v8/include)
                file(REMOVE_RECURSE ${ROOT}/include)
                message(FATAL_ERROR "Unable to find v8 headers")
            endif ()
            file(COPY ${ENV_BIND_NODE_HEADERS_PATH}/deps/v8/include/
                 DESTINATION ${ROOT}/include)
            if (NOT EXISTS ${ENV_BIND_NODE_HEADERS_PATH}/deps/zlib)
                file(REMOVE_RECURSE ${ROOT}/include)
                message(FATAL_ERROR "Unable to find zlib headers")
            endif ()
            file(COPY ${ENV_BIND_NODE_HEADERS_PATH}/deps/zlib/
                 DESTINATION ${ROOT}/include)
        endif ()
        file(REMOVE_RECURSE ${ENV_BIND_NODE_HEADERS_PATH})
        unset(ENV_BIND_NODE_HEADERS_PATH CACHE)
    endif ()


    # OS specific

    # Only download the libraries on windows, since its the only place
    # its necessary. Note, this requires rerunning CMake if moving
    # a module from one platform to another (should happen automatically
    # with most generators)
    if (WIN32)
        # Download the win32 library for linking
        file(STRINGS
             ${ROOT}/CHECKSUM LIB32_CHECKSUM
             LIMIT_COUNT 1
             REGEX ${__env_bind_node_lib32_regex})
        if (NOT LIB32_CHECKSUM)
            message(FATAL_ERROR "Unable to extract x86 library checksum")
        endif ()
        string(REGEX MATCH ${__env_bind_node_lib32_regex} LIB32_CHECKSUM ${LIB32_CHECKSUM})
        set(LIB32_CHECKSUM ${CMAKE_MATCH_1})
        set(LIB32_PATH win-x86)
        set(LIB32_NAME ${CMAKE_MATCH_4}${CMAKE_MATCH_5})
        set(LIB32_TARGET ${CMAKE_MATCH_2}${CMAKE_MATCH_3}${LIB32_NAME})
        if (NOT EXISTS ${ROOT}/${LIB32_PATH})
            file(REMOVE_RECURSE ${TEMP}/${LIB32_PATH})
            env_bind_node_download_file(
                    ${URL}/${VERSION}/${LIB32_TARGET}
                    ${TEMP}/${LIB32_PATH}/${LIB32_NAME}
                    INACTIVITY_TIMEOUT 10
                    EXPECTED_HASH ${CHECK_TYPE}=${LIB32_CHECKSUM}
                    STATUS LIB32_STATUS)
            list(GET LIB32_STATUS 0 LIB32_STATUS)
            if (LIB32_STATUS GREATER 0)
                message(FATAL_ERROR
                        "Unable to download Node.js windows library (32-bit)")
            endif ()
            file(REMOVE_RECURSE ${ROOT}/${LIB32_PATH})
            file(MAKE_DIRECTORY ${ROOT}/${LIB32_PATH})
            file(RENAME
                 ${TEMP}/${LIB32_PATH}/${LIB32_NAME}
                 ${ROOT}/${LIB32_PATH}/${LIB32_NAME})
            file(REMOVE_RECURSE ${TEMP}/${LIB32_PATH})
        endif ()

        # Download the win64 library for linking
        file(STRINGS
             ${ROOT}/CHECKSUM LIB64_CHECKSUM
             LIMIT_COUNT 1
             REGEX ${__env_bind_node_lib64_match})
        if (NOT LIB64_CHECKSUM)
            message(FATAL_ERROR "Unable to extract x64 library checksum")
        endif ()
        string(REGEX MATCH ${__env_bind_node_lib64_match} LIB64_CHECKSUM ${LIB64_CHECKSUM})
        set(LIB64_CHECKSUM ${CMAKE_MATCH_1})
        set(LIB64_PATH win-x64)
        set(LIB64_NAME ${CMAKE_MATCH_4}${CMAKE_MATCH_5})
        set(LIB64_TARGET ${CMAKE_MATCH_2}${CMAKE_MATCH_3}${LIB64_NAME})
        if (NOT EXISTS ${ROOT}/${LIB64_PATH})
            file(REMOVE_RECURSE ${TEMP}/${LIB64_PATH})
            env_bind_node_download_file(
                    ${URL}/${VERSION}/${LIB64_TARGET}
                    ${TEMP}/${LIB64_PATH}/${LIB64_NAME}
                    INACTIVITY_TIMEOUT 10
                    EXPECTED_HASH ${CHECK_TYPE}=${LIB64_CHECKSUM}
                    STATUS LIB64_STATUS)
            list(GET LIB64_STATUS 0 LIB64_STATUS)
            if (LIB64_STATUS GREATER 0)
                message(FATAL_ERROR
                        "Unable to download Node.js windows library (64-bit)")
            endif ()
            file(REMOVE_RECURSE ${ROOT}/${LIB64_PATH})
            file(MAKE_DIRECTORY ${ROOT}/${LIB64_PATH})
            file(RENAME
                 ${TEMP}/${LIB64_PATH}/${LIB64_NAME}
                 ${ROOT}/${LIB64_PATH}/${LIB64_NAME}
                 )
            file(REMOVE_RECURSE ${TEMP}/${LIB64_PATH})
        endif ()
    endif ()

    # The downloaded headers should always be set for inclusion
    list(APPEND INCLUDE_DIRS ${ROOT}/include)

    # Look for the NAN module, and add it to the includes
    env_bind_node_find_module(nan ${CMAKE_CURRENT_SOURCE_DIR} ENV_BIND_NODE_NAN_DIR)
    if (ENV_BIND_NODE_NAN_DIR)
        list(APPEND INCLUDE_DIRS ${ENV_BIND_NODE_NAN_DIR})
    endif ()

    # Under windows, we need a bunch of libraries (due to the way
    # dynamic linking works)
    if (WIN32)
        # Generate and use a delay load hook to allow the node binary
        # name to be changed while still loading native modules
        set(DELAY_LOAD_HOOK ${CMAKE_CURRENT_BINARY_DIR}/win_delay_load_hook.c)
        nodejs_generate_delayload_hook(${DELAY_LOAD_HOOK})
        set(SOURCES "${PROJECT_SOURCE_DIR}")

        # Necessary flags to get delayload working correctly
        list(APPEND LINK_FLAGS
             "-IGNORE:4199"
             "-DELAYLOAD:iojs.exe"
             "-DELAYLOAD:node.exe"
             "-DELAYLOAD:node.dll")

        # Core system libraries used by node.
        list(APPEND LIBRARIES
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

        # Also link to the node stub itself (downloaded above).
        if (CMAKE_CL_64)
            list(APPEND LIBRARIES
                 "${ROOT}/${LIB64_PATH}/${LIB64_NAME}")
        else ()
            list(APPEND LIBRARIES
                 "${ROOT}/${LIB32_PATH}/${LIB32_NAME}")
        endif ()
    else ()
        # Non-windows platforms should use these flags.
        list(APPEND DEFINITIONS
             "_LARGEFILE_SOURCE"
             "_FILE_OFFSET_BITS=64")
    endif ()

    # Special handling for OSX / clang to allow undefined symbols.
    # Define is required by node on OSX.
    if (APPLE)
        list(APPEND LINK_FLAGS "-undefined dynamic_lookup")
        list(APPEND DEFINITIONS _DARWIN_USE_64_BIT_INODE=1)
    endif ()


    # Export

    set(ENV_BIND_NODE_VERSION ${VERSION} CACHE STRING
        "Version of NodeJS to use for linking into Node bindings."
        FORCE)

    set(ENV_BIND_NODE_SOURCES ${SOURCES} CACHE STRING
        "Sources of NodeJS to use for linking into Node bindings."
        FORCE)

    set(ENV_BIND_NODE_INCLUDE_DIRS ${INCLUDE_DIRS} CACHE STRING
        "Include directories of NodeJS to use for linking into Node bindings."
        FORCE)

    set(ENV_BIND_NODE_LIBRARIES ${LIBRARIES} CACHE STRING
        "Libraries of NodeJS to use for linking into Node bindings."
        FORCE)

    set(ENV_BIND_NODE_LINK_FLAGS ${LINK_FLAGS} CACHE STRING
        "Link flags to use for Node bindings."
        FORCE)

    set(ENV_BIND_NODE_DEFINITIONS ${DEFINITIONS} CACHE STRING
        "Definitions to use for Node bindings."
        FORCE)


    # Prevents this function from executing more than once.

    set(ENV_BIND_NODE_INITIALIZED TRUE CACHE BOOL
        "Indicates whether NodeJS bindings are initialized."
        FORCE)
endfunction()


# Add binding -----------------------------------------------------------------

# Helper function for defining a node module.
# All of the settings and dependencies necessary to do
# this yourself are defined, but this helps make sure everything is configured
# correctly. Feel free to use it as a model to do this by hand (or to
# tweak this configuration if you need something custom).

function(env_bind_node_add _name)
    env_bind_node_initialize()

    # Validate name parameter (must be a valid C identifier).
    string(MAKE_C_IDENTIFIER ${_name} ${_name}_SYMBOL_CHECK)
    if (NOT "${_name}" STREQUAL "${${_name}_SYMBOL_CHECK}")
        message(FATAL_ERROR
                "Module name must be a valid C identifier. "
                "Suggested alternative: '${${_name}_SYMBOL_CHECK}'")
    endif ()

    # In order to match node-gyp, we need to build into type specific folders
    # ncmake takes care of this, but be sure to set CMAKE_BUILD_TYPE yourself
    # if invoking CMake directly.
    if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
        message(FATAL_ERROR
                "Configuration type must be specified. "
                "Set CMAKE_BUILD_TYPE or use a different generator")
    endif ()

    # A node module is a shared library.
    add_library(${_name} SHARED ${ENV_BIND_NODE_SOURCES} ${ARGN})

    # Add compiler defines for the module.
    # Two helpful ones:
    # MODULE__name must match the name of the build library, define that here.
    # ${_name}_BUILD is for symbol visibility under windows.
    string(TOUPPER "${_name}_BUILD" ${_name}_BUILD_DEF)
    target_compile_definitions(
            ${_name}
            PRIVATE MODULE__name=${_name}
            PRIVATE ${${_name}_BUILD_DEF}
            PUBLIC ${ENV_BIND_NODE_DEFINITIONS})

    # This properly defines includes for the module.
    target_include_directories(${_name} PUBLIC ${ENV_BIND_NODE_INCLUDE_DIRS})

    # Add link flags to the module.
    target_link_libraries(${_name} ${ENV_BIND_NODE_LIBRARIES})

    # Set required properties for the module to build properly.
    # Correct naming, symbol visiblity and C++ standard.
    set_target_properties(
            ${_name}
            PROPERTIES
            OUTPUT__name ${_name}
            PREFIX ""
            SUFFIX ".node"
            MACOSX_RPATH ON
            C_VISIBILITY_PRESET hidden
            CXX_VISIBILITY_PRESET hidden
            POSITION_INDEPENDENT_CODE TRUE
            CMAKE_CXX_STANDARD_REQUIRED TRUE
            CXX_STANDARD 11)

    # Handle link flag cases properly.
    # When there are link flags, they should be appended to LINK_FLAGS
    # with space separation.
    # If the list is emtpy (true for most *NIX platforms), this is a no-op.
    foreach (ENV_BIND_NODE_LINK_FLAG IN LISTS ENV_BIND_NODE_LINK_FLAGS)
        set_property(
                TARGET ${_name}
                APPEND_STRING PROPERTY
                LINK_FLAGS " ${ENV_BIND_NODE_LINK_FLAG}")
    endforeach ()

    # Make sure we're buiilding in a build specific output directory.
    # Only necessary on single-target generators (Make, Ninja).
    # Multi-target generators do this automatically.
    # This (luckily) mirrors node-gyp conventions.
    if (NOT CMAKE_CONFIGURATION_TYPES)
        set_property(
                TARGET ${_name}
                PROPERTY
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE})
    endif ()
endfunction()
