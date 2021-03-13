include(FetchContent)
set(env_fetch_directory ${FETCHCONTENT_BASE_DIR})
set(FETCHCONTENT_BASE_DIR ${env_fetch_directory})

function(env_fetch _name)
    env_log("Fetching \"${_name}\".")

    FetchContent_Declare(env_${_name}_fetch ${ARGN})
    FetchContent_MakeAvailable(env_${_name}_fetch)
endfunction()

macro(env_get_fetch_properties _name)
    FetchContent_GetProperties(env_${_name}_fetch ${ARGN})
endmacro()
