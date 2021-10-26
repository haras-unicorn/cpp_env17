include_guard()

include(EnvFunc)


# Versions --------------------------------------------------------------------

function(env_node_detect_installed_version _out)
    set(_res 1)
    find_program(_binary NAMES node nodejs)
    if (_binary)
        env_log(NodeJS installation detected.)

        execute_process(
                COMMAND ${_binary} --version
                RESULT_VARIABLE _res
                OUTPUT_VARIABLE _version
                OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif ()

    if (_res STREQUAL 0)
        env_log(Installed NodeJS version is \"${_version}\".)
    else ()
        set(_version "")
        env_log(Installed NodeJS version not detected.)
    endif ()

    set(${_out} "${_version}" PARENT_SCOPE)
endfunction()

function(env_node_resolve_checksums_version
         _version
         _installed_version
         _fallback_version
         _out)
    if (_version STREQUAL installed)
        if (_installed_version)
            set(_checksums_version "${_installed_version}")
        else ()
            set(_checksums_version "${_fallback_version}")
        endif ()
    else ()
        set(_checksums_version "${_version}")
    endif ()
    env_log(Resolved NodeJS checksums version \"${_checksums_version}\".)

    set(${_out} "${_checksums_version}" PARENT_SCOPE)
endfunction()


# Checksums file --------------------------------------------------------------

function(env_node_fetch_checksums_file _url _name _out)
    env_fetch(checksums
              FROM
              URL "${_url}"
              NO_EXTRACT

              INTO node)

    env_use_lower_project_name()
    string(JOIN / _file
           "${${LOWER_PROJECT_NAME}_checksums_fetch_src_dir}"
           "${_name}")
    env_log(NodeJS checksums file fetched in \"${_file}\".)

    set(${_out} "${_file}" PARENT_SCOPE)
endfunction()


set(__env_node_checksums_headers_archive_regex
    [[^([A-Fa-f0-9]+)[\t ]+([^-]+)-(headers|v?[0-9.]+)-(headers|v?[0-9.]+)([.]tar[.]gz)$]]
    CACHE STRING
    "Regex for matching NodeJS components from checksums file."
    FORCE)

function(env_node_extract_headers_archive_components
         _checksums_file
         _out_name
         _out_version
         _out_id
         _out_checksum
         _out_rpath)

    file(STRINGS "${_checksums_file}"
         _checksum_line
         REGEX ${__env_node_checksums_headers_archive_regex}
         LIMIT_COUNT 1)

    if (NOT _checksum_line)
        env_log(FATAL_ERROR Unable to extract NodeJS headers' archive checksum.)
    endif ()

    string(REGEX MATCH
           "${__env_node_checksums_headers_archive_regex}"
           _checksum_match
           "${_checksum_line}")


    set(_name "${CMAKE_MATCH_2}")
    set(${_out_name} "${_name}" PARENT_SCOPE)

    if (CMAKE_MATCH_3 STREQUAL headers)
        set(_version "${CMAKE_MATCH_4}")
    else ()
        set(_version "${CMAKE_MATCH_3}")
    endif ()
    set(${_out_version} "${_version}" PARENT_SCOPE)

    set(_id "${_name}-${_version}")
    set(${_out_id} "${_id}" PARENT_SCOPE)
    env_log(NodeJS ID used for headers' archive is
            \"${_id}\".)


    set(_checksum "${CMAKE_MATCH_1}")
    set(${_out_checksum} "${_checksum}" PARENT_SCOPE)
    env_log(NodeJS headers' archive checksum is
            \"${_checksum}\".)

    string(JOIN "" _rpath
           "${_version}/"
           "${CMAKE_MATCH_2}-${CMAKE_MATCH_3}-${CMAKE_MATCH_4}"
           "${CMAKE_MATCH_5}")
    set(${_out_rpath} "${_rpath}" PARENT_SCOPE)
    env_log(NodeJS headers' archive relative path is
            \"${_rpath}\".)
endfunction()


set(__env_node_checksums_win_lib32_regex
    [[(^[0-9A-Fa-f]+)[\t ]+(win-x86)?(/)?([^/]*)(.lib)]]
    CACHE STRING
    "Regex for matching NodeJS Windows 32 bit library components from its name."
    FORCE)

function(env_node_extract_win_lib32_components
         _checksums_file
         _out_checksum
         _out_version_rpath)

    file(STRINGS "${_checksums_file}"
         _checksum_line
         REGEX ${__env_node_checksums_win_lib32_regex}
         LIMIT_COUNT 1)

    if (NOT _checksum_line)
        env_log(FATAL_ERROR
                Unable to extract NodeJS Windows 32 bit library checksum.)
    endif ()

    string(REGEX MATCH
           "${__env_node_checksums_win_lib32_regex}"
           _checksum_match
           "${_checksum_line}")


    set(_checksum "${CMAKE_MATCH_1}")
    set(${_out_checksum} "${_checksum}" PARENT_SCOPE)
    env_log(NodeJS Windows 32 bit library checksum is
            \"${_checksum}\".)

    string(JOIN "" _version_rpath
           "${CMAKE_MATCH_2}"
           "${CMAKE_MATCH_3}"
           "${CMAKE_MATCH_4}"
           "${CMAKE_MATCH_5}")
    set(${_out_version_rpath} "${_version_rpath}" PARENT_SCOPE)
    env_log(NodeJS Windows 32 bit library version relative path is
            \"${_version_rpath}\".)
endfunction()


set(__env_node_checksums_win_lib64_regex
    [[(^[0-9A-Fa-f]+)[\t ]+(win-)?(x64/)(.*)(.lib)$]]
    CACHE STRING
    "Regex for matching Windows NodeJS 64 bit library components from its name."
    FORCE)

function(env_node_extract_win_lib64_components
         _checksums_file
         _out_checksum
         _out_version_rpath)

    file(STRINGS "${_checksums_file}"
         _checksum_line
         REGEX ${__env_node_checksums_win_lib64_regex}
         LIMIT_COUNT 1)

    if (NOT _checksum_line)
        env_log(FATAL_ERROR
                Unable to extract NodeJS Windows 64 bit library checksum.)
    endif ()

    string(REGEX MATCH
           "${__env_node_checksums_win_lib64_regex}"
           _checksum_match
           "${_checksum_line}")


    set(_checksum "${CMAKE_MATCH_1}")
    set(${_out_checksum} "${_checksum}" PARENT_SCOPE)
    env_log(NodeJS Windows 64 bit library checksum is
            \"${_checksum}\".)

    string(JOIN "" _version_rpath
           "${CMAKE_MATCH_2}"
           "${CMAKE_MATCH_3}"
           "${CMAKE_MATCH_4}"
           "${CMAKE_MATCH_5}")
    set(${_out_version_rpath} "${_version_rpath}" PARENT_SCOPE)
    env_log(NodeJS Windows 64 bit library version relative path is
            \"${_version_rpath}\".)
endfunction()


# Component fetch -------------------------------------------------------------

function(env_node_fetch_headers _url _check_type _checksum)
    env_fetch(headers
              FROM
              URL "${_url}"
              URL_HASH "${_check_type}=${_checksum}"

              INTO node

              SCAFFOLD
              INCLUDE_DIRS "/include/node")

    env_use_lower_project_name()
    env_log(NodeJS headers' archive fetched in
            \"${${LOWER_PROJECT_NAME}_headers_fetch_src_dir}\")
endfunction()

function(env_node_fetch_win_lib _url _check_type _checksum)
    env_fetch(win_lib
              FROM
              URL "${_url}"
              URL_HASH "${_check_type}=${_checksum}"
              NO_EXTRACT

              INTO node

              SCAFFOLD
              LIB_GLOB "/*.lib")

    env_use_lower_project_name()
    env_log(NodeJS Windows library fetched in
            \"${${LOWER_PROJECT_NAME}_win_lib_src_dir}\")
endfunction()


# OS specific interface -------------------------------------------------------

function(env_node_add_os_specific_interface
         _win_delay_load_hook_src
         _node_win_lib)

    env_log(Adding for NodeJS bindings OS specific target.)
    env_add_export(os)

    if (ENV_WIN)
        env_use_lower_project_name()
        add_library(
                ${LOWER_PROJECT_NAME}_win_delay_load_hook
                OBJECT
                "${_win_delay_load_hook_src}")

        env_add_alias(win_delay_load_hook)

        env_target_link(
                os
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
                "DelayImp.lib"
                ${LOWER_PROJECT_NAME}::win_delay_load_hook
                ${_node_win_lib})

        env_target_link_with(
                os
                INTERFACE
                "-IGNORE:4199"
                "-DELAYLOAD:iojs.exe"
                "-DELAYLOAD:node.exe"
                "-DELAYLOAD:node.dll")

    else ()
        env_target_definitions(
                os
                INTERFACE
                "_LARGEFILE_SOURCE"
                "_FILE_OFFSET_BITS=64")
    endif ()

    if (ENV_IOS OR ENV_MACOS)
        env_target_link_with(
                os
                INTERFACE
                "-undefined dynamic_lookup")

        env_target_definitions(
                os
                INTERFACE
                "_DARWIN_USE_64_BIT_INODE=1")
    endif ()
endfunction()


# Bindings --------------------------------------------------------------------

function(env_node_decorate_bind _name)
    env_use_upper_project_name()
    env_prefix_with_project_name(${_name} _mod)
    env_log(Decorating NodeJS binding \"${_name}\".)

    string(MAKE_C_IDENTIFIER ${_mod} _sym_check)
    if (NOT "${_mod}" STREQUAL "${_sym_check}")
        env_log(FATAL_ERROR
                NodeJS binding name name must be a valid C identifier.
                Suggested alternative \"${_sym_check}\".)
    endif ()


    env_target_set(
            ${_mod}
            SUFFIX ".node"
            MACOSX_RPATH ON)

    string(TOUPPER "${_mod}_BUILD" _build_def)
    env_target_definitions(
            ${_mod}
            PRIVATE
            "MODULE__name=${_mod}"
            "${_build_def}")

    env_macro_name(${_mod} _prefix)
    env_target_definitions(
            ${_mod}
            PRIVATE
            "${_prefix}_NODE_BIND=1"
            "${UPPER_PROJECT_NAME}_NODE_BIND=1"
            "ENV_NODE_BIND=1")


    env_target_set_bin_output(${_mod})
endfunction()
