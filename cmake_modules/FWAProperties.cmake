include(FWALog)

# add code
function(fwa_target_link _name)
    fwa_log("Linking \"${_name}\" with \"${ARGN}\".")
    target_link_libraries(fwa_${_name} ${ARGN})
endfunction()

function(fwa_target_include _name)
    fwa_log("Into \"${_name}\" including \"${ARGN}\".")
    target_include_directories(fwa_${_name} ${ARGN})
endfunction()

function(fwa_target_sources _name)
    fwa_log("Sourcing \"${_name}\" with \"${ARGN}\".")
    target_sources(fwa_${_name} ${ARGN})
endfunction()


# properties

function(fwa_target_set _name)
    fwa_log("On \"${_name}\" setting \"${ARGN}\".")
    set_target_properties(fwa_${_name} PROPERTIES ${ARGN})
endfunction()

include(CheckIPOSupported)
check_ipo_supported(RESULT fwa_ipo_supported)
if(fwa_ipo_supported)
    fwa_log("IPO is supported.")

    function(fwa_target_set_ipo _name)
        fwa_target_set(${_name} INTERPROCEDURAL_OPTIMIZATION ON)
    endfunction()
else()
    fwa_log("IPO is not supported.")

    function(fwa_target_set_ipo _name)
    endfunction()
endif()


# compilation

function(fwa_target_precompile _name)
    fwa_log("Precompiling \"${_name}\" with \"${ARGN}\".")
    target_precompile_headers(fwa_${_name} ${ARGN})
endfunction()

function(fwa_target_compile _name)
    fwa_log("Compiling \"${_name}\" with \"${ARGN}\".")
    target_compile_features(fwa_${_name} ${ARGN})
endfunction()

function(fwa_target_definitions _name)
    fwa_log("Compiling \"${_name}\" with \"${ARGN}\".")
    target_compile_definitions(fwa_${_name} ${ARGN})
endfunction()

if (MSVC)
    function(fwa_target_warnings _name)
        fwa_log("Adding warnings to \"${_name}\".")
        target_compile_options(fwa_${_name} PRIVATE /W4 /WX /permissive- /Zc:__cplusplus)
    endfunction()
else()
    function(fwa_target_warnings _name)
        fwa_log("Adding warnings to \"${_name}\".")
        target_compile_options(fwa_${_name} PRIVATE -Wall -Wextra -pedantic -Werror)
    endfunction()
endif()


# targets

function(fwa_add_dependency _name)
    fwa_log("Adding \"${_name}\" dependency.")

    add_library(fwa_${_name} ${ARGN})
endfunction()

function(fwa_add_test _name)
    fwa_log("Adding \"${_name}\" test.")

    add_executable(fwa_${_name}_test ${PROJECT_SOURCE_DIR}/src/fwa_${_name}/fwa_${_name}_tests.cpp)
    fwa_target_link(${_name}_test PRIVATE ${ARGN} PRIVATE fwa_default fwa_doctest)

    fwa_target_set(${_name}_test CXX_EXTENSIONS OFF)
    fwa_target_set(${_name}_test POSITION_INDEPENDENT_CODE ON)
    fwa_target_set_ipo(${_name}_test)
    fwa_target_warnings(${_name}_test)

    add_test(NAME fwa_${_name}_test COMMAND fwa_${_name}_test)
endfunction()

function(fwa_add_addon _name)
    fwa_log("Adding \"${_name}\" addon.")

    add_library(fwa_${_name}_addon MODULE ${PROJECT_SOURCE_DIR}/src/fwa_${_name}/fwa_${_name}_addon.cpp)
    fwa_target_link(${_name}_addon PRIVATE ${ARGN} PRIVATE fwa_default)

    fwa_target_set(${_name}_addon CXX_EXTENSIONS OFF)
    fwa_target_set(${_name}_addon POSITION_INDEPENDENT_CODE ON)
    fwa_target_set_ipo(${_name}_addon)
    fwa_target_warnings(${_name}_addon)
endfunction()
