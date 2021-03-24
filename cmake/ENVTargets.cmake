include(ENVLog)
include(ENVName)
include(ENVCompiler)
include(ENVSources)
include(ENVProperties)
include(ENVCompilation)
include(ENVWarnings)
include(ENVOptimization)


# atomic targets

function(env_add_executable _name)
    env_prefix(${_name} env _name)
    env_log("Adding \"${_name}\" executable.")

    add_executable(${_name} ${ARGN})
endfunction()

function(env_add_library _name)
    env_prefix(${_name} env _name)

    env_log("Adding \"${_name}\" library.")
    add_library(${_name} ${ARGN})
endfunction()

function(env_add_alias _name)
    env_prefix(${_name} env _prefixed)

    env_log("Adding \"env::${_name}\" alias.")
    add_library(env::${_name} ALIAS ${_prefixed})
endfunction()


# compound targets

function(env_add_dep _name)
    env_suffix(${_name} dep _name)
    env_log(" - Adding \"${_name}\" dependency. - ")

    env_add_library(${_name} INTERFACE IMPORTED)
endfunction()

function(env_add_test _name _src)
    env_suffix(${_name} test _name)
    env_log(" - Adding \"${_name}\" test. - ")

    env_add_executable(${_name} ${_src})
    env_target_link(${_name} PRIVATE ${ARGN} PRIVATE env_default_dep)

    env_target_set(${_name} CXX_EXTENSIONS OFF)
    env_target_link_options(${_name} "-v")
    env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
    env_target_set_ipo(${_name})

    env_target_warn(${_name})
    env_target_optimize(${_name})

    add_test(NAME env_${_name} COMMAND env_${_name})
endfunction()

if (NOT CMAKE_BUILD_TYPE STREQUAL Debug) # Doesn't make much sense to benchmark debug
    function(env_add_bench _name _src)
        env_suffix(${_name} bench _name)
        env_log(" - Adding \"${_name}\" benchmark. - ")

        env_add_executable(${_name} ${_src})
        env_target_link(${_name} PRIVATE ${ARGN} PRIVATE env_default_dep)

        env_target_set(${_name} CXX_EXTENSIONS OFF)
        env_target_link_options(${_name} "-v")
        env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
        env_target_set_ipo(${_name})

        env_target_warn(${_name})
        env_target_optimize(${_name})
    endfunction()
else ()
    function(env_add_bench)
    endfunction()
endif ()

function(env_add_export _name)
    env_log(" - Adding \"${_name}\" export. - ")

    env_add_library(${_name} INTERFACE)
    env_add_alias(${_name})
    env_target_link(${_name} INTERFACE ${ARGN} INTERFACE env_default_dep)
    env_target_include(${_name} INTERFACE ${PROJECT_SOURCE_DIR}/include)
endfunction()
