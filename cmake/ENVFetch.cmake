include(FetchContent)
set(env_fetch_dir ${FETCHCONTENT_BASE_DIR})
set(FETCHCONTENT_BASE_DIR ${env_fetch_dir})


function(env_fetch _name)
    env_prefix(${_name} env _name)
    env_suffix(${_name} fetch _name)

    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/builds/fetch/${_name}_src/.cmake_populated.txt")
        env_log("Fetching \"${_name}\" into \"${PROJECT_SOURCE_DIR}/builds/fetch/${_name}_src\".")
        FetchContent_Populate(
                ${_name}
                QUIET
                ${ARGN}
                SOURCE_DIR "${PROJECT_SOURCE_DIR}/builds/fetch/${_name}_src"
                BINARY_DIR "${PROJECT_BINARY_DIR}/fetch/${_name}/bin"
                SUBBUILD_DIR "${PROJECT_BINARY_DIR}/fetch/${_name}/sub"
        )

        file(WRITE "${PROJECT_SOURCE_DIR}/builds/fetch/${_name}_src/.cmake_populated.txt" YES)
    endif()

    add_subdirectory(
            "${PROJECT_SOURCE_DIR}/builds/fetch/${_name}_src"
            "${PROJECT_BINARY_DIR}/fetch/${_name}/bin"
    )
endfunction()
