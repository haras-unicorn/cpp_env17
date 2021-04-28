include_guard()


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
        env_log(Installed NodeJS version is: \"${_version}\".)
    else ()
        set(_version "")
        env_log(Installed NodeJS version not detected.)
    endif ()

    set(${_out} "${_version}" PARENT_SCOPE)
endfunction()

function(env_node_resolve_checksums_version _version _installed_version _out)
    if (_node_version STREQUAL installed AND
        _installed_version)
        set(_checksums_version "${_installed_version}")
    else ()
        set(_checksums_version "${_version}")
    endif ()
    env_log(Resolved NodeJS checksums version: \"${_checksums_version}\".)

    set(${_out} "${_checksums_version}" PARENT_SCOPE)
endfunction()


# Checksums file --------------------------------------------------------------

function(env_node_fetch_checksums_file _url _name _out)
    env_fetch(checksums URL "${_url}" DOWNLOAD_NO_EXTRACT TRUE)

    env_use_lower_project_name()
    string(JOIN / _file "${${LOWER_PROJECT_NAME}_checksums_src_dir}" "${_name}")
    env_log(NodeJS checksums file fetched in: \"${_file}\".)

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


    set(${_out_name} "${CMAKE_MATCH_2}" PARENT_SCOPE)

    if (CMAKE_MATCH_3 STREQUAL headers)
        set(_version "${CMAKE_MATCH_4}" PARENT_SCOPE)
    else ()
        set(_version "${CMAKE_MATCH_3}" PARENT_SCOPE)
    endif ()
    set(${_out_version} "${_version}" PARENT_SCOPE)

    set(_id "${_out_name}-${_out_version}")
    set(${_out_id} "${_id}" PARENT_SCOPE)
    env_log(NodeJS ID used for headers' archive is:
            \"${_id}\".)


    set(_checksum "${CMAKE_MATCH_1}")
    set(${_out_checksum} "_checksum" PARENT_SCOPE)
    env_log(NodeJS headers' archive checksum is:
            \"${_checksum}\".)

    string(JOIN "" _rpath
           "${_version}/"
           "${CMAKE_MATCH_2}-${CMAKE_MATCH_3}-${CMAKE_MATCH_4}"
           "${CMAKE_MATCH_5}")
    set(${_out_rpath} "${_rpath}" PARENT_SCOPE)
    env_log(NodeJS headers' archive relative path is:
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
    set(${_out_checksum} "_checksum" PARENT_SCOPE)
    env_log(NodeJS Windows 32 bit library checksum is:
            \"${_checksum}\".)

    string(JOIN "" _version_rpath
           "${CMAKE_MATCH_2}"
           "${CMAKE_MATCH_3}"
           "${CMAKE_MATCH_4}"
           "${CMAKE_MATCH_5}")
    set(${_out_version_rpath} "${_version_rpath}" PARENT_SCOPE)
    env_log(NodeJS Windows 32 bit library version relative path is:
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
    set(${_out_checksum} "_checksum" PARENT_SCOPE)
    env_log(NodeJS Windows 64 bit library checksum is:
            \"${_checksum}\".)

    string(JOIN "" _version_rpath
           "${CMAKE_MATCH_2}"
           "${CMAKE_MATCH_3}"
           "${CMAKE_MATCH_4}"
           "${CMAKE_MATCH_5}")
    set(${_out_version_rpath} "${_version_rpath}" PARENT_SCOPE)
    env_log(NodeJS Windows 64 bit library version relative path is:
            \"${_version_rpath}\".)
endfunction()


# Component fetch -------------------------------------------------------------

function(env_node_fetch_headers _name _url _check_type _checksum _id)
    env_fetch(${_name}
              URL "${_url}"
              URL_HASH "${_check_type}=${_checksum}"

              SCAFFOLD
              INCLUDE_DIRS "/${_id}/include")

    env_use_lower_project_name()
    env_log(NodeJS headers' archive fetched in:
            \"${${LOWER_PROJECT_NAME}_${_name}_src_dir}\")
endfunction()

function(env_node_fetch_win_lib _name _url _check_type _checksum)
    env_fetch(${_name}
              URL "${_url}"
              URL_HASH "${_check_type}=${_checksum}"

              DOWNLOAD_NO_EXTRACT TRUE

              SCAFFOLD
              LIB_GLOB "/*.lib")

    env_use_lower_project_name()
    env_log(NodeJS Windows library \"${_name}\" fetched in:
            \"${${LOWER_PROJECT_NAME}_${_name}_src_dir}\")
endfunction()
