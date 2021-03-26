if (CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    if (ENV_CLANG_CL)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    /O2
            )
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    /O2
            )
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    -O3
            )
        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding optimizations to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    -O3
            )
        endfunction()
    else ()
        function(env_target_optimize _name)
        endfunction()
    endif ()
else ()
    if (ENV_CLANG_CL)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    # TODO: fix ClangCL complains
                    # /ZI # debug info
                    # /fsanitize=address
            )
        endfunction()
    elseif (ENV_MSVC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    /ZI # debug info
                    /fsanitize=address
            )
        endfunction()
    elseif (ENV_GCC)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    -Og
                    -ggdb
                    -fsanitize=address,leak,undefined
            )
        endfunction()
    elseif (ENV_CLANG)
        function(env_target_optimize _name)
            env_prefix(${_name} env _name)
            env_log("Adding sanitization to \"${_name}\".")

            target_compile_options(
                    ${_name}
                    PRIVATE
                    -ggdb
                    # TODO fix
                    # -fsanitize=address,undefined
            )
        endfunction()
    else ()
        function(env_target_optimize _name)
        endfunction()
    endif ()
endif ()
