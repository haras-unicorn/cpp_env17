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
