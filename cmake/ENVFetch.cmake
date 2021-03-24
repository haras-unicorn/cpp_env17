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
