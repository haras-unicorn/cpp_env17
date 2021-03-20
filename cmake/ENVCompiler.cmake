if (MSVC) # for some reason "CMAKE_CXX_COMPILER_ID STREQUAL MSVC" doesn't work
    env_log("Detected MSVC compiler.")
    set(ENV_MSVC TRUE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    env_log("Detected GCC compiler.")
    set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC TRUE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    env_log("Detected Clang compiler.")
    set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG TRUE CACHE BOOL "Whether Clang was detected or not.")

else ()
    env_log("Unknown compiler.")
    set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
    set(ENV_GCC FALSE CACHE BOOL "Whether GCC was detected or not.")
    set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")
endif ()
