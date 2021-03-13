include(ENVLog)

# add code
function(env_target_link _name)
    env_log("Linking \"${_name}\" with \"${ARGN}\".")
    target_link_libraries(env_${_name} ${ARGN})
endfunction()

function(env_target_include _name)
    env_log("Into \"${_name}\" including \"${ARGN}\".")
    target_include_directories(env_${_name} ${ARGN})
endfunction()

function(env_target_sources _name)
    env_log("Sourcing \"${_name}\" with \"${ARGN}\".")
    target_sources(env_${_name} ${ARGN})
endfunction()


# properties

function(env_target_set _name)
    env_log("On \"${_name}\" setting \"${ARGN}\".")
    set_target_properties(env_${_name} PROPERTIES ${ARGN})
endfunction()

include(CheckIPOSupported)
check_ipo_supported(RESULT env_ipo_supported)
if(env_ipo_supported)
    env_log("IPO is supported.")

    function(env_target_set_ipo _name)
        env_target_set(${_name} INTERPROCEDURAL_OPTIMIZATION ON)
    endfunction()
else()
    env_log("IPO is not supported.")

    function(env_target_set_ipo _name)
    endfunction()
endif()


# compilation

function(env_target_precompile _name)
    env_log("Precompiling \"${_name}\" with \"${ARGN}\".")
    target_precompile_headers(env_${_name} ${ARGN})
endfunction()

function(env_target_compile _name)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")
    target_compile_features(env_${_name} ${ARGN})
endfunction()

function(env_target_definitions _name)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")
    target_compile_definitions(env_${_name} ${ARGN})
endfunction()

if (MSVC)
    function(env_target_warnings _name)
        env_log("Adding warnings to \"${_name}\".")
        target_compile_options(env_${_name} PRIVATE /W4 /WX /permissive- /Zc:__cplusplus)
    endfunction()
else()
    function(env_target_warnings _name)
        env_log("Adding warnings to \"${_name}\".")
        target_compile_options(env_${_name} PRIVATE -Wall -Wextra -pedantic -Werror)
    endfunction()
endif()


# targets

function(env_add_dependency _name)
    env_log("Adding \"${_name}\" dependency.")

    add_library(env_${_name} ${ARGN})
endfunction()

function(env_add_test _name)
    env_log("Adding \"${_name}\" test.")

    add_executable(env_${_name}_test ${PROJECT_SOURCE_DIR}/src/env_${_name}/env_${_name}_tests.cpp)
    env_target_link(${_name}_test PRIVATE ${ARGN} PRIVATE env_default env_doctest)

    env_target_set(${_name}_test CXX_EXTENSIONS OFF)
    env_target_set(${_name}_test POSITION_INDEPENDENT_CODE ON)
    env_target_set_ipo(${_name}_test)
    env_target_warnings(${_name}_test)

    add_test(NAME env_${_name}_test COMMAND env_${_name}_test)
endfunction()

function(env_add_addon _name)
    env_log("Adding \"${_name}\" addon.")

    add_library(env_${_name}_addon MODULE ${PROJECT_SOURCE_DIR}/src/env_${_name}/env_${_name}_addon.cpp)
    env_target_link(${_name}_addon PRIVATE ${ARGN} PRIVATE env_default)

    env_target_set(${_name}_addon CXX_EXTENSIONS OFF)
    env_target_set(${_name}_addon POSITION_INDEPENDENT_CODE ON)
    env_target_set_ipo(${_name}_addon)
    env_target_warnings(${_name}_addon)
endfunction()
