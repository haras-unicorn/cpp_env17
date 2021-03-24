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
