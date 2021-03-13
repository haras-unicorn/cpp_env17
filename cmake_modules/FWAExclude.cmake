function(fwa_prune_dependencies)
    foreach(_dependency ${ARGN})
        string(REGEX MATCH fwa_ _is_fwa ${_dependency})
        get_target_property(_type ${_dependency} TYPE)

        if (NOT "${_is_fwa}" 
            AND NOT ${_type} STREQUAL INTERFACE_LIBRARY
            AND NOT ${_type} STREQUAL OBJECT_LIBRARY
            AND NOT ${_type} STREQUAL MODULE_LIBRARY)

            fwa_log("Excluding target \"${_dependency}\".")
            set_target_properties(${_dependency} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        endif()
    endforeach()
endfunction()

function(fwa_prune_directories)
    foreach(_directory ${ARGN})
        get_directory_property(_dependencies DIRECTORY ${_directory} BUILDSYSTEM_TARGETS)
        fwa_prune_dependencies(${_dependencies})

        get_directory_property(_subdirectories DIRECTORY ${_directory} SUBDIRECTORIES)
        fwa_prune_directories(${_subdirectories})
    endforeach()    
endfunction()
