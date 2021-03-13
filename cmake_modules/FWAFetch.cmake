include(FetchContent)
set(fwa_fetch_directory ${FETCHCONTENT_BASE_DIR})
set(FETCHCONTENT_BASE_DIR ${fwa_fetch_directory})

function(fwa_fetch _name)
    fwa_log("Fetching \"${_name}\".")

    FetchContent_Declare(fwa_${_name}_fetch ${ARGN})
    FetchContent_MakeAvailable(fwa_${_name}_fetch)
endfunction()

macro(fwa_get_fetch_properties _name)
    FetchContent_GetProperties(fwa_${_name}_fetch ${ARGN})
endmacro()
