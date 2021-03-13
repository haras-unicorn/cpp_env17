include(ENVLog)
include(ENVName)


# add code

function(env_target_link _name)
    env_prefix(${_name} env _name)
    env_log("Linking \"${_name}\" with \"${ARGN}\".")

    target_link_libraries(${_name} ${ARGN})
endfunction()

function(env_target_include _name)
    env_prefix(${_name} env _name)
    env_log("Into \"${_name}\" including \"${ARGN}\".")

    target_include_directories(${_name} ${ARGN})
endfunction()

function(env_target_sources _name)
    env_prefix(${_name} env _name)
    env_log("Sourcing \"${_name}\" with \"${ARGN}\".")

    target_sources(${_name} ${ARGN})
endfunction()


# properties

function(env_target_set _name)
    env_prefix(${_name} env _name)
    env_log("On \"${_name}\" setting \"${ARGN}\".")

    set_target_properties(${_name} PROPERTIES ${ARGN})
endfunction()

include(CheckIPOSupported)
check_ipo_supported(RESULT env_ipo_supported)
if (env_ipo_supported)
    env_log("IPO is supported.")

    function(env_target_set_ipo _name)
        env_target_set(${_name} INTERPROCEDURAL_OPTIMIZATION ON)
    endfunction()
else ()
    env_log("IPO is not supported.")

    function(env_target_set_ipo _name)
    endfunction()
endif ()


# compilation

function(env_target_precompile _name)
    env_prefix(${_name} env _name)
    env_log("Precompiling \"${_name}\" with \"${ARGN}\".")

    target_precompile_headers(${_name} ${ARGN})
endfunction()

function(env_target_compile _name)
    env_prefix(${_name} env _name)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")

    target_compile_features(${_name} ${ARGN})
endfunction()

function(env_target_definitions _name)
    env_prefix(${_name} env _name)
    env_log("Compiling \"${_name}\" with \"${ARGN}\".")

    target_compile_definitions(${_name} ${ARGN})
endfunction()


# warnings

if (MSVC)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(${_name} PRIVATE /W4 /WX /permissive- /Zc:__cplusplus)
    endfunction()
elseif (GCC)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(${_name} PRIVATE -Wall -Wextra -pedantic -Werror -ftrack-macro-expansion=0)
    endfunction()
elseif (CLANG)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(${_name} PRIVATE -Wall -Wextra -pedantic -Werror)
    endfunction()
else ()
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")
    endfunction()
endif ()


# optimizations

if (CMAKE_BUILD_TYPE STREQUAL Release)
    if (MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(${_name} PRIVATE /O2)
        endfunction()
    elseif (GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(${_name} PRIVATE -O3)
        endfunction()
    elseif (CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(${_name} PRIVATE -O3)
        endfunction()
    else ()
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")
        endfunction()
    endif ()
else ()
    function(env_target_optimize _name)
        env_prefix(${_name} env _name)
        env_log("Adding optimizations to \"${_name}\".")
    endfunction()
endif ()


# atomic targets

function(env_add_executable _name)
    env_prefix(${_name} env _name)
    env_log("Adding \"${_name}\" executable.")

    add_executable(${_name} ${ARGN})
endfunction()

function(env_add_library _name)
    env_prefix(${_name} env _prefixed)
    env_log("Adding \"${_prefixed}\" library.")

    add_library(${_prefixed} ${ARGN})
    add_library(env::${_name} ALIAS ${_prefixed})
endfunction()


# compound targets

function(env_add_dep _name)
    env_suffix(${_name} dep _name)
    env_log(" - Adding \"${_name}\" dependency. - ")

    env_add_library(${_name} INTERFACE)
endfunction()

function(env_add_test _name _src)
    env_suffix("${_name}" test _name)
    env_log(" - Adding \"${_name}\" test. - ")

    env_add_executable(${_name} ${_src})
    env_target_link(${_name} PRIVATE ${ARGN} PRIVATE env_default_dep env_test_dep env_bench_dep)

    env_target_set(${_name} CXX_EXTENSIONS OFF)
    env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
    env_target_set_ipo(${_name})

    env_target_warn(${_name})

    add_test(NAME env_${_name} COMMAND env_${_name})
endfunction()

function(env_add_bench _name _src)
    env_suffix("${_name}" bench _name)
    env_log(" - Adding \"${_name}\" benchmark. - ")

    env_add_executable(${_name} ${_src})
    env_target_link(${_name} PRIVATE ${ARGN} PRIVATE env_default_dep env_test_dep env_bench_dep)

    env_target_set(${_name} CXX_EXTENSIONS OFF)
    env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
    env_target_set_ipo(${_name})

    env_target_warn(${_name})
    env_target_optimize(${_name})
endfunction()
