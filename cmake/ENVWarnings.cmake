if (ENV_CLANG_CL)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
                PRIVATE
                /W4 /WX
                # used separately and would probably take a long time
                # --analyze
                /permissive- # standards compliance
                /Zc:__cplusplus # otherwise we can't detect the C++ standard
        )
    endfunction()
elseif (ENV_MSVC)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
                PRIVATE
                /W4 /WX
                /analyze
                /permissive- # standards compliance
                /Zc:__cplusplus # otherwise we can't detect the C++ standard
        )
    endfunction()
elseif (ENV_GCC)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
                PRIVATE
                -Wall -Wextra -Wpedantic -Werror
                # takes too long - run when library is done?
                # -fanalyzer
                -ftrack-macro-expansion=0 # so messages are printed nicely
                -Wno-multichar # detect endianness
        )
    endfunction()
elseif (ENV_CLANG)
    function(env_target_warn _name)
        env_prefix(${_name} env _name)
        env_log("Adding warnings to \"${_name}\".")

        target_compile_options(
                ${_name}
                PRIVATE
                # used separately and would probably take a long time
                # --analyze
                -Wall -Wextra -Wpedantic -Werror
        )
    endfunction()
else ()
    function(env_target_warn _name)
    endfunction()
endif ()
