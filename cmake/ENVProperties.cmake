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

function(env_target_link_options _name)
    env_prefix(${_name} env _name)
    env_log("On \"${_name}\" adding link options \"${ARGN}\".")

    target_link_options(${_name} PRIVATE ${ARGN})
endfunction()
