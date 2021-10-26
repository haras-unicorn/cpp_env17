# Guard -----------------------------------------------------------------------

if(ENV_FUNCTIONS_INCLUDED)
  return()
endif()

set(ENV_FUNCTIONS_INCLUDED TRUE)


# TODO: fix diamond dependencies and versioning somehow
# semver git tags?

# TODO: use EXPORT_COMPILE_COMMAND in 3.20


# -----------------------------------------------------------------------------
# Utilities
# -----------------------------------------------------------------------------

# Names -----------------------------------------------------------------------

function(env_prefix _name _prefix _out)
  string(REGEX MATCH "^${_prefix}" _match ${_name})
  if(_name AND NOT _match)
    set(${_out} "${_prefix}_${_name}" PARENT_SCOPE)
  elseif(_match)
    set(${_out} "${_name}" PARENT_SCOPE)
  else()
    set(${_out} "${_prefix}" PARENT_SCOPE)
  endif()
endfunction()

function(env_suffix _name _suffix _out)
  string(REGEX MATCH "${_suffix}\$" _match ${_name})
  if(_name AND NOT _match)
    set(${_out} "${_name}_${_suffix}" PARENT_SCOPE)
  elseif(_match)
    set(${_out} "${_name}" PARENT_SCOPE)
  else()
    set(${_out} "${_suffix}" PARENT_SCOPE)
  endif()
endfunction()


set(
  __env_delimiter_regex
  [[\.|-|_|/|\\|::]]
  CACHE STRING
  "Matches delimiters."
  FORCE
)

set(
  __env_extension_regex
  [[\..*$]]
  CACHE STRING
  "Matches file extensions."
  FORCE
)

function(env_name_format _name _out)
  string(REGEX REPLACE "${__env_extension_regex}" "" _name "${_name}")
  string(REGEX REPLACE "${__env_delimiter_regex}" _ _name "${_name}")
  set(${_out} ${_name} PARENT_SCOPE)
endfunction()

function(env_normalize_name _name _out)
  env_name_format(${_name} _name)
  string(TOLOWER ${_name} _name)
  set(${_out} ${_name} PARENT_SCOPE)
endfunction()

function(env_macro_name _name _out)
  env_name_format(${_name} _name)
  string(TOUPPER ${_name} _name)
  set(${_out} ${_name} PARENT_SCOPE)
endfunction()


function(env_use_upper_project_name)
  env_name_format(${PROJECT_NAME} _upper)
  string(TOUPPER ${_upper} _upper)
  set(UPPER_PROJECT_NAME ${_upper} PARENT_SCOPE)
endfunction()

function(env_use_lower_project_name)
  env_name_format(${PROJECT_NAME} _lower)
  string(TOLOWER ${_lower} _lower)
  set(LOWER_PROJECT_NAME ${_lower} PARENT_SCOPE)
endfunction()

function(env_prefix_with_project_name _name _out)
  env_use_lower_project_name()
  env_prefix(${_name} ${LOWER_PROJECT_NAME} _prefixed)
  set(${_out} ${_prefixed} PARENT_SCOPE)
endfunction()

function(env_target_name_for _path _out)
  file(RELATIVE_PATH _relative "${PROJECT_SOURCE_DIR}" "${_path}")
  env_normalize_name(${_relative} _name)

  env_use_lower_project_name()
  set(${_out} ${LOWER_PROJECT_NAME}_${_name} PARENT_SCOPE)
endfunction()


function(env_has_extension _path _out)
  string(REGEX REPLACE "${__env_extension_regex}" "" _no_ex "${_path}")

  if(_path STREQUAL ${_no_ex})
    set(${_out} FALSE PARENT_SCOPE)
  else()
    set(${_out} TRUE PARENT_SCOPE)
  endif()
endfunction()


function(env_escape_backslash _string _out)
  string(REGEX REPLACE [[\\]] [[\\\\]] _res "${_string}")
  set(${_out} "${_res}" PARENT_SCOPE)
endfunction()


# Logging ---------------------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  option(
    ENV_LOG_VERBOSE
    "Turn on verbose CMake logging."
    ON
  )
else()
  option(
    ENV_LOG_VERBOSE
    "Turn on verbose CMake ON."
    OFF
  )
endif()

set(
  __env_verbose_message_levels
  "\
STATUS;VERBOSE;DEBUG;TRACE\
"
  CACHE STRING
  "Message levels considered verbose by the environment."
  FORCE
)

set(
  __env_message_levels
  "\
FATAL_ERROR;SEND_ERROR;WARNING;AUTHOR_WARNING;DEPRECATION;NOTICE;\
${env_verbose_message_levels}\
"
  CACHE STRING
  "Valid message levels for environment."
  FORCE
)

if(ENV_LOG_VERBOSE)
  function(env_log _level)
    list(FIND __env_message_levels ${_level} _index)

    list(JOIN ARGN " " _message)
    env_use_lower_project_name()
    if(NOT _index EQUAL -1)
      message(${_level} "[env::${LOWER_PROJECT_NAME}]: ${_message}")
    else()
      message(STATUS "[env::${LOWER_PROJECT_NAME}]: ${_level} ${_message}")
    endif()
  endfunction()
else()
  function(env_log _level)
    list(FIND __env_verbose_message_levels ${_level} _index)

    list(JOIN ARGN " " _message)
    if(NOT _index EQUAL -1)
      env_use_lower_project_name()
      message(${_level} "[env::${LOWER_PROJECT_NAME}]: ${_message}")
    endif()
  endfunction()
endif()


# Set -------------------------------------------------------------------------

macro(env_null_set _name)
  if(NOT ${_name} OR ${_name} STREQUAL ${_name}-NOTFOUND)
    set(${_name} ${ARGN})
  endif()
endmacro()


# -----------------------------------------------------------------------------
# Detection
# -----------------------------------------------------------------------------

env_log(-!- Running detection. -!-)


# Generator -------------------------------------------------------------------

env_log(- Detecting generator. -)
env_log(CMake generator is \"${CMAKE_GENERATOR}\".)
env_log(CMake generator toolset is \"${CMAKE_GENERATOR_TOOLSET}\".)


# Compiler --------------------------------------------------------------------

# TODO: Proper Intel compiler support

# TODO: Add ARMCC, ARMClang and NVIDIA CUDA compiler support


env_log(- Detecting compiler. -)


env_log(CMake Compiler ID is \"${CMAKE_CXX_COMPILER_ID}\".)
env_log(CMake MSVC is present \"${MSVC}\".)


# "CMAKE_CXX_COMPILER_ID STREQUAL MSVC" doesn't always work for MSVC

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  if(MSVC)
    env_log(Detected ClangCl compiler.)
    set(
      ENV_CLANG_CL
      TRUE CACHE BOOL
      "Whether CLANG_CL was detected or not."
      FORCE
    )

    set(ENV_COMPILER "clang_cl" CACHE STRING "Compiler name." FORCE)

  else()
    env_log(Detected Clang compiler.)
    set(
      ENV_CLANG
      TRUE CACHE BOOL
      "Whether Clang was detected or not."
      FORCE
    )

    set(ENV_COMPILER "clang" CACHE STRING "Compiler name." FORCE)
  endif()
elseif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL MSVC OR MSVC)
  env_log(Detected MSVC compiler.)
  set(
    ENV_MSVC
    TRUE CACHE BOOL
    "Whether MSVC was detected or not."
    FORCE
  )

  set(ENV_COMPILER "msvc" CACHE STRING "Compiler name." FORCE)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  env_log(Detected GNU compiler.)
  set(
    ENV_GNU
    TRUE CACHE BOOL
    "Whether GNU was detected or not."
    FORCE
  )

  set(ENV_COMPILER "gnu" CACHE STRING "Compiler name." FORCE)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL Intel)
  env_log(Detected Intel compiler.)
  set(
    ENV_INTEL
    TRUE CACHE BOOL
    "Whether Intel was detected or not."
    FORCE
  )

  set(ENV_COMPILER "intel" CACHE STRING "Compiler name." FORCE)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL IntelLLVM)
  env_log(Detected Intel LLVM compiler.)
  set(
    ENV_INTEL_LLVM
    TRUE CACHE BOOL
    "Whether Intel LLVM was detected or not."
    FORCE
  )

  set(ENV_COMPILER "intel_llvm" CACHE STRING "Compiler name." FORCE)

else()
  env_log(FATAL_ERROR Unknown compiler.)

endif()


set(ENV_CLANG_CL FALSE CACHE BOOL "Whether ClangCL was detected or not.")
set(ENV_MSVC FALSE CACHE BOOL "Whether MSVC was detected or not.")
set(ENV_GNU FALSE CACHE BOOL "Whether GCC was detected or not.")
set(ENV_CLANG FALSE CACHE BOOL "Whether Clang was detected or not.")
set(ENV_INTEL FALSE CACHE BOOL "Whether Intel was detected or not.")
set(ENV_INTEL_LLVM FALSE CACHE BOOL "Whether Intel LLVM was detected or not.")


# Hardware --------------------------------------------------------------------

env_log(- Detecting hardware. -)


env_log(CMake sizeof\(void*\) = ${CMAKE_SIZEOF_VOID_P}.)

set(__env_cxx_byte_order "${CMAKE_CXX_BYTE_ORDER}" CACHE "CXX byte order." FORCE)
set(__env_c_byte_order "${CMAKE_C_BYTE_ORDER}" CACHE "C byte order." FORCE)

env_log(CMake CXX byte order is \"${__env_cxx_byte_order}\".)
env_log(CMake C byte order is \"${__env_c_byte_order}\".)

env_log(CMake system processor is \"${CMAKE_SYSTEM_PROCESSOR}\".)


if(CMAKE_SIZEOF_VOID_P STREQUAL 4)
  env_log(Address size is 32 bit.)
  set(
    ENV_32BIT
    TRUE CACHE BOOL
    "Whether address size is 32 bit."
    FORCE
  )

elseif(CMAKE_SIZEOF_VOID_P STREQUAL 8)
  env_log(Address size is 64 bit.)
  set(
    ENV_64BIT
    TRUE CACHE BOOL
    "Whether address size is 64 bit."
    FORCE
  )

elseif()
  env_log(FATAL_ERROR Unsupported address size.)

endif()

set(ENV_32BIT FALSE CACHE BOOL "Whether architecture is 32 bit.")
set(ENV_64BIT FALSE CACHE BOOL "Whether architecture is 64 bit.")


set(ENV_CXX_BYTE_ORDER "${__env_cxx_byte_order}" CACHE "CXX byte order." FORCE)
set(ENV_C_BYTE_ORDER "${__env_c_byte_order}" CACHE "C byte order." FORCE)

if(__env_cxx_byte_order STREQUAL BIG_ENDIAN)
  env_log(CXX is big endian.)
  set(
    ENV_CXX_BIG_ENDIAN
    TRUE CACHE BOOL
    "Whether architecture is big endian."
    FORCE
  )
else()
  env_log(CXX is little endian.)
  set(
    ENV_CXX_LITTLE_ENDIAN
    TRUE CACHE BOOL
    "Whether architecture is little endian."
    FORCE
  )
endif()

set(ENV_CXX_BIG_ENDIAN FALSE CACHE BOOL "Whether architecture is big endian.")
set(ENV_CXX_LITTLE_ENDIAN FALSE CACHE BOOL "Whether architecture is little endian.")

if(__env_c_byte_order STREQUAL BIG_ENDIAN)
  env_log(C is big endian.)
  set(
    ENV_C_BIG_ENDIAN
    TRUE CACHE BOOL
    "Whether architecture is big endian."
    FORCE
  )
else()
  env_log(C is little endian.)
  set(
    ENV_C_LITTLE_ENDIAN
    TRUE CACHE BOOL
    "Whether architecture is little endian."
    FORCE
  )
endif()

set(ENV_C_BIG_ENDIAN FALSE CACHE BOOL "Whether architecture is big endian.")
set(ENV_C_LITTLE_ENDIAN FALSE CACHE BOOL "Whether architecture is little endian.")


if(CMAKE_SYSTEM_PROCESSOR STREQUAL AMD64)
  env_log(Architecture is amd64.)
  set(
    ENV_AMD64
    TRUE CACHE BOOL
    "Whether the architecture is amd64."
    FORCE
  )

  set(ENV_ARCH "amd64" CACHE STRING "Architecture name." FORCE)

elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL x86)
  env_log(Architecture is x86.)
  set(
    ENV_X86
    TRUE CACHE BOOL
    "Whether the architecture is x86."
    FORCE
  )

  set(ENV_ARCH "x86" CACHE STRING "Architecture name." FORCE)

elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
  env_log(Architecture is arm.)
  set(
    ENV_ARM
    TRUE CACHE BOOL
    "Whether the processor is arm."
    FORCE
  )

  set(ENV_ARCH "arm" CACHE STRING "Architecture name." FORCE)

elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL arm64)
  env_log(Architecture is arm64.)
  set(
    ENV_ARM64
    TRUE CACHE BOOL
    "Whether the processor is arm64."
    FORCE
  )

  set(ENV_ARCH "arm64" CACHE STRING "Architecture name." FORCE)

else()
  env_log(FATAL_ERROR Unsupported architecture.)

endif()

set(ENV_AMD64 FALSE CACHE BOOL "Whether architecture is amd64.")
set(ENV_X86 FALSE CACHE BOOL "Whether architecture is x86.")
set(ENV_ARM FALSE CACHE BOOL "Whether architecture is arm.")
set(ENV_ARM64 FALSE CACHE BOOL "Whether architecture is arm64.")


# OS --------------------------------------------------------------------------

env_log(- Detecting OS. -)


env_log(CMake system is \"${CMAKE_SYSTEM}\".)
env_log(CMake Windows is present \"${WIN32}\".)
env_log(CMake Android is present \"${ANDROID}\".)
env_log(CMake Apple is present \"${APPLE}\".)
env_log(CMake Linux is present \"${LINUX}\".)


if(WIN32)
  env_log(OS is Windows.)
  set(
    ENV_WIN
    TRUE CACHE BOOL
    "Whether OS is Windows."
    FORCE
  )

  set(ENV_OS "win" CACHE STRING "OS name." FORCE)

elseif(ANDROID)
  env_log(OS is Android.)
  set(
    ENV_ANDROID
    TRUE CACHE BOOL
    "Whether OS is Android."
    FORCE
  )

  set(ENV_OS "android" CACHE STRING "OS name." FORCE)

elseif(CMAKE_SYSTEM_NAME Darwin)
  env_log(OS is macOS.)
  set(
    ENV_MACOS
    TRUE CACHE BOOL
    "Whether OS is macOS."
    FORCE
  )

  set(ENV_OS "macos" CACHE STRING "OS name." FORCE)

elseif(CMAKE_SYSTEM_NAME iOS)
  env_log(OS is iOS.)
  set(
    ENV_IOS
    TRUE CACHE BOOL
    "Whether OS is iOS."
    FORCE
  )

  set(ENV_OS "ios" CACHE STRING "OS name." FORCE)

elseif(UNIX AND NOT APPLE)
  env_log(OS is Linux.)
  set(
    ENV_LINUX
    TRUE CACHE BOOL
    "Whether OS is Linux."
    FORCE
  )

  set(ENV_OS "linux" CACHE STRING "OS name." FORCE)

else()
  env_log(FATAL_ERROR "Unsupported OS.")

endif()


set(ENV_WIN FALSE CACHE BOOL "Whether OS is Windows.")
set(ENV_LINUX FALSE CACHE BOOL "Whether OS is Linux.")
set(ENV_MACOS FALSE CACHE BOOL "Whether OS is macOS.")

set(ENV_ANDROID FALSE CACHE BOOL "Whether OS is Android.")
set(ENV_IOS FALSE CACHE BOOL "Whether OS is iOS.")


# -----------------------------------------------------------------------------
# Target configuration
# -----------------------------------------------------------------------------

# Sources ---------------------------------------------------------------------

function(env_target_include _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(Into \"${_name}\" including \"${ARGN}\".)

  target_include_directories(${_mod} PRIVATE ${ARGN})
endfunction()

function(env_target_link _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(Linking \"${_name}\" with \"${ARGN}\".)

  target_link_libraries(${_mod} PRIVATE ${ARGN})
endfunction()

function(env_target_sources _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(Sourcing \"${_name}\" with \"${ARGN}\".)

  target_sources(${_mod} PRIVATE ${ARGN})
endfunction()


# Properties ------------------------------------------------------------------

function(env_target_set _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(On \"${_name}\" setting \"${ARGN}\".)

  set_target_properties(${_mod} PROPERTIES ${ARGN})
endfunction()


include(CheckIPOSupported)
check_ipo_supported(RESULT __env_ipo_supported LANGUAGES CXX C)

set(
  ENV_IPO_SUPPORTED
  ${__env_ipo_supported}
  CACHE BOOL
  "Whether interprocedural optimization is supported."
  FORCE
)

if(ENV_IPO_SUPPORTED)
  env_log(Interprocedural optimization is supported.)

  function(env_target_set_ipo _name)
    env_target_set(${_name} INTERPROCEDURAL_OPTIMIZATION ON)
  endfunction()
else()
  env_log(Interprocedural optimization is not supported.)

  function(env_target_set_ipo _name)
  endfunction()
endif()


include(CheckPIESupported)
check_pie_supported(OUTPUT_VARIABLE __env_pie_supported LANGUAGES CXX C)

set(
  ENV_PIE_SUPPORTED
  ${__env_pie_supported}
  CACHE BOOL
  "Whether position independent code is supported."
  FORCE
)

if(ENV_PIE_SUPPORTED)
  env_log(Position independent code is supported.)

  function(env_target_set_pie _name)
    env_target_set(${_name} POSITION_INDEPENDENT_CODE ON)
  endfunction()
else()
  env_log(Position independent code is not supported.)

  function(env_target_set_pie _name)
  endfunction()
endif()

function(env_set_visibility _name _type)
  env_prefix_with_project_name(${_name} _mod)

  if(_type STREQUAL DEFAULT)
    env_log(Setting default visibility on \"${_name}\".)
    set_target_properties(
      ${_mod}
      PROPERTIES
      CXX_VISIBILITY_PRESET default
      C_VISIBILITY_PRESET default
    )

  elseif(_type STREQUAL INLINES_HIDDEN)
    env_log(Setting inlines hidden visibility on \"${_name}\".)
    set_target_properties(
      ${_mod}
      PROPERTIES
      CXX_VISIBILITY_PRESET default
      C_VISIBILITY_PRESET default
      VISIBILITY_INLINES_HIDDEN TRUE
    )

  elseif(_type STREQUAL HIDDEN)
    env_log(Setting hidden visibility on \"${_name}\".)
    set_target_properties(
      ${_mod}
      PROPERTIES
      CXX_VISIBILITY_PRESET hidden
      C_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN TRUE
    )

  endif()
endfunction()


set(
  __env_binary_root
  "${CMAKE_SOURCE_DIR}/.bin"
  CACHE STRING
  "Root of all binary target outputs."
  FORCE
)

set(
  __env_binary_prefix
  "${ENV_OS}_${ENV_ARCH}_${ENV_COMPILER}"
  CACHE STRING
  "\
Prefix that encodes OS, architecture and compiler information into a \
target binary output file."
  FORCE
)

function(env_target_resolve_bin_out_dir _target _out)
  cmake_parse_arguments(PARSED "" "SUB" "" ${ARGN})

  if(NOT PARSED_SUB)
    env_use_lower_project_name()
    set(PARSED_SUB "${LOWER_PROJECT_NAME}")
  endif()

  string(JOIN / _dir "${__env_binary_root}" "${PARSED_SUB}")

  set(${_out} ${_dir} PARENT_SCOPE)
endfunction()

function(env_target_resolve_bin_out_name _target _out)
  env_name_format(${_target} _name)
  set(${_out} "${__env_binary_prefix}_${_name}" PARENT_SCOPE)
endfunction()

function(env_target_set_bin_output _target)
  cmake_parse_arguments(PARSED "" "SUB" "" ${ARGN})

  env_target_resolve_bin_out_dir(${_target} _dir SUB "${PARSED_SUB}")
  env_target_resolve_bin_out_name(${_target} _name)

  env_log(
    Setting binary output directory on \"${_target}\"
    to \"${_dir}\"
    and output name to \"${_name}\".
  )

  get_target_property(_type ${_target} TYPE)
  if(_type STREQUAL MODULE_LIBRARY)
    env_target_set(
      ${_target}
      LIBRARY_OUTPUT_DIRECTORY "${_dir}"
      OUTPUT_NAME "${_name}"
    )
  elseif(_type STREQUAL SHARED_LIBRARY)
    if(ENV_WIN)
      env_target_set(
        ${_target}
        RUNTIME_OUTPUT_DIRECTORY "${_dir}"
        ARCHIVE_OUTPUT_DIRECTORY "${_dir}"
        OUTPUT_NAME "${_name}"
      )
    else()
      env_target_set(
        ${_target}
        LIBRARY_OUTPUT_DIRECTORY "${_dir}"
        OUTPUT_NAME "${_name}"
      )
    endif()
  elseif(_type STREQUAL STATIC_LIBRARY)
    env_target_set(
      ${_target}
      ARCHIVE_OUTPUT_DIRECTORY "${_dir}"
      OUTPUT_NAME "${_name}"
    )
  elseif(_type STREQUAL EXECUTABLE)
    env_target_set(
      ${_target}
      RUNTIME_OUTPUT_DIRECTORY "${_dir}"
      OUTPUT_NAME "${_name}"
    )
  endif()
endfunction()


function(env_target_get_location _target _out)
  get_target_property(_type ${_target} TYPE)

  if(_type STREQUAL MODULE_LIBRARY)
    get_target_property(
      _config_dir ${_target}
      LIBRARY_OUTPUT_DIRECTORY
    )
    get_target_property(
      _dir ${_target}
      LIBRARY_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}
    )
    env_null_set(_dir "${_config_dir}")

  elseif(_type STREQUAL SHARED_LIBRARY)
    if(ENV_WIN)
      get_target_property(
        _config_dir ${_target}
        ARCHIVE_OUTPUT_DIRECTORY
      )
      get_target_property(
        _dir ${_target}
        ARCHIVE_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}
      )
      env_null_set(_dir "${_config_dir}")

    else()
      get_target_property(
        _config_dir ${_target}
        LIBRARY_OUTPUT_DIRECTORY
      )
      get_target_property(
        _dir ${_target}
        LIBRARY_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}
      )
      env_null_set(_dir "${_config_dir}")

    endif()
  elseif(_type STREQUAL STATIC_LIBRARY)
    get_target_property(
      _config_dir ${_target}
      ARCHIVE_OUTPUT_DIRECTORY
    )
    get_target_property(
      _dir ${_target}
      ARCHIVE_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}
    )
    env_null_set(_dir "${_config_dir}")

  elseif(_type STREQUAL EXECUTABLE)
    get_target_property(
      _config_dir ${_target}
      RUNTIME_OUTPUT_DIRECTORY
    )
    get_target_property(
      _dir ${_target}
      RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}
    )
    env_null_set(_dir "${_config_dir}")

  endif()

  get_target_property(_name ${_target} OUTPUT_NAME)
  get_target_property(_prefix ${_target} PREFIX)
  get_target_property(_suffix ${_target} SUFFIX)

  if(_prefix STREQUAL _prefix-NOTFOUND)
    set(_prefix "")
  endif()
  if(_suffix STREQUAL _suffix-NOTFOUND)
    set(_suffix "")
  endif()

  set(${_out} "${_dir}/${_prefix}${_name}${_suffix}" PARENT_SCOPE)
endfunction()

function(env_target_get_native_location _target _out)
  env_target_get_location(${_target} _location)
  file(TO_NATIVE_PATH "${_location}" _native)
  set(${_out} "${_native}" PARENT_SCOPE)
endfunction()


# Flags -----------------------------------------------------------------------

function(env_target_compile_with _name _visibility)
  env_prefix_with_project_name(${_name} _mod)
  env_log(On \"${_name}\" adding compile options \"${ARGN}\".)

  target_compile_options(${_mod} ${ARGN})
endfunction()

include(CheckCXXCompilerFlag)
function(env_target_safely_compile_with _name)
  cmake_parse_arguments(PARSED "" "" "PRIVATE;PUBLIC;INTERFACE" ${ARGN})

  env_prefix_with_project_name(${_name} _mod)
  env_log(On \"${_name}\" adding compile options \"${ARGN}\".)


  foreach(_flag IN LISTS PARSED_PRIVATE)
    check_cxx_compiler_flag(${_flag} _supported)
    if(_supported)
      target_compile_options(${_mod} PRIVATE ${_flag})
    else()
      env_log(
        WARNING
        On \"${_name}\" adding compile option \"${_flag}\"
        FAILED. REASON: Option not supported by compiler.
      )
    endif()
  endforeach()

  foreach(_flag IN LISTS PARSED_PUBLIC)
    check_cxx_compiler_flag(${_flag} _supported)
    if(_supported)
      target_compile_options(${_mod} PUBLIC ${_flag})
    else()
      env_log(
        WARNING
        On \"${_name}\" adding compile option \"${_flag}\"
        FAILED. REASON: Option not supported by compiler.
      )
    endif()
  endforeach()

  foreach(_flag IN LISTS PARSED_INTERFACE)
    check_cxx_compiler_flag(${_flag} _supported)
    if(_supported)
      target_compile_options(${_mod} INTERFACE ${_flag})
    else()
      env_log(
        WARNING
        On \"${_name}\" adding compile option \"${_flag}\"
        FAILED. REASON: Option not supported by compiler.
      )
    endif()
  endforeach()
endfunction()


function(env_target_link_with _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(On \"${_name}\" adding link options \"${ARGN}\".)

  target_link_options(${_mod} ${ARGN})
endfunction()


# Compilation -----------------------------------------------------------------

function(env_target_features _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(Compiling \"${_name}\" with \"${ARGN}\".)

  target_compile_features(${_mod} ${ARGN})
endfunction()

if(ENV_CLANG_CL OR ENV_MSVC)
  function(env_target_conform _name)
    env_log(Setting standards conformance on \"${_name}\".)

    target_compile_options(
      ${_name}
      PRIVATE
      # standards conformance
      /permissive-
      # otherwise we can't detect the C++ standard
      /Zc:__cplusplus
      # MSVC and ClangCL are weird about auto
      /Zc:auto
    )

    # clang-cl and sometimes MSVC don't enable exceptions by default
    # unlike all other compilers

    get_target_property(_options ${_name} COMPILE_OPTIONS)
    if(_options STREQUAL _options-NOTFOUND)
      target_compile_options(${_name} PRIVATE /EHsc)
    else()
      string(REGEX MATCH [[/EH.*]] _exceptions_enabled ${_options})
      if(NOT _exceptions_enabled)
        target_compile_options(${_name} PRIVATE /EHsc)
      else()
        env_log(Exceptions already enabled on \"${_name}\".)
      endif()
    endif()
  endfunction()
else()
  function(env_target_conform _name)
  endfunction()
endif()

function(env_set_cpp17 _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(Setting C++17 and C11 standard on \"${_name}\".)

  target_compile_features(
    ${_mod}
    PRIVATE
    cxx_std_17
    c_std_11
  )

  set_target_properties(
    ${_mod}
    PROPERTIES
    CXX_EXTENSIONS OFF
    C_EXTENSIONS OFF
  )
endfunction()


function(env_target_definitions _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(Compiling \"${_name}\" with \"${ARGN}\".)

  target_compile_definitions(${_mod} ${ARGN})
endfunction()


function(env_target_precompile _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log(Precompiling \"${_name}\" with \"${ARGN}\".)

  target_precompile_headers(${_mod} PRIVATE ${ARGN})
endfunction()


# Warnings --------------------------------------------------------------------

# TODO: clang analysis
# deeply copy the original target and
# add the deep copy as a dependency with the --analyze flag?

if(ENV_CLANG_CL)
  function(env_target_warn _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding warnings to \"${_name}\".)

    target_compile_options(${_mod} PRIVATE /W4 /WX)
  endfunction()
  function(env_target_suppress _name)
    env_log(Suppressing warnings on \"${_name}\".)

    target_compile_options(${_name} PRIVATE /w)
  endfunction()

elseif(ENV_MSVC)
  function(env_target_warn _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding warnings to \"${_name}\".)

    target_compile_options(${_mod} PRIVATE /W4 /WX /analyze)
  endfunction()
  function(env_target_suppress _name)
    env_log(Suppressing warnings on \"${_name}\".)

    target_compile_options(${_name} PRIVATE /w)
  endfunction()

elseif(ENV_GNU)
  function(env_target_warn _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log("Adding warnings to \"${_name}\".")

    target_compile_options(
      ${_mod}
      PRIVATE
      -Wall -Wextra -Wpedantic -Werror
      -fanalyzer
      # so messages are printed nicely
      -ftrack-macro-expansion=0
    )
  endfunction()
  function(env_target_suppress _name)
    env_log(Suppressing warnings on \"${_name}\".)

    target_compile_options(${_name} PRIVATE -w)
  endfunction()

elseif(ENV_CLANG)
  function(env_target_warn _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding warnings to \"${_name}\".)

    target_compile_options(${_mod} PRIVATE -Wall -Wextra -Wpedantic -Werror)
  endfunction()
  function(env_target_suppress _name)
    env_log(Suppressing warnings on \"${_name}\".)

    target_compile_options(${_name} PRIVATE -w)
  endfunction()

else()
  function(env_target_warn)
  endfunction()
  function(env_target_suppress)
  endfunction()

endif()


set(
  __env_warning_regex [[/W.*|-W.*]]
  CACHE STRING
  "Regex that matches compiler warning options."
  FORCE
)

function(env_target_clear_warn _name)
  env_log(Clearing warnings from \"${_name}\".)

  get_target_property(_options ${_name} COMPILE_OPTIONS)
  if(NOT _options STREQUAL _options-NOTFOUND)
    list(FILTER _options EXCLUDE REGEX ${__env_warning_regex})
    set_target_properties(
      ${_name}
      PROPERTIES
      COMPILE_OPTIONS "${_options}"
    )
  endif()

  get_target_property(_interface_options ${_name} INTERFACE_COMPILE_OPTIONS)
  if(NOT _interface_options STREQUAL _interface_options-NOTFOUND)
    list(FILTER _interface_options EXCLUDE REGEX ${__env_warning_regex})
    set_target_properties(
      ${_name}
      PROPERTIES
      INTERFACE_COMPILE_OPTIONS "${_interface_options}"
    )
  endif()
endfunction()


# Optimization ----------------------------------------------------------------

# TODO: sanitization and valgrind

if(CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
  if(ENV_CLANG_CL)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding optimizations to \"${_name}\".)

      target_compile_options(${_mod} PRIVATE /O2)
      env_target_set_ipo(${_name})
    endfunction()
  elseif(ENV_MSVC)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding optimizations to \"${_name}\".)

      target_compile_options(${_mod} PRIVATE /O2)
      env_target_set_ipo(${_mod})
    endfunction()
  elseif(ENV_GNU)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding optimizations to \"${_name}\".)

      target_compile_options(${_mod} PRIVATE -O3)
      env_target_set_ipo(${_mod})
    endfunction()
  elseif(ENV_CLANG)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding optimizations to \"${_name}\".)

      target_compile_options(${_mod} PRIVATE -O3)
      env_target_set_ipo(${_mod})
    endfunction()
  else()
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding optimizations to \"${_name}\".)

      env_target_set_ipo(${_mod})
    endfunction()
  endif()
else()
  if(ENV_CLANG_CL)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding sanitization to \"${_name}\".)

      target_compile_options(
        ${_mod}
        PRIVATE
        /Zi
        # -fsanitize=address,undefined
        # /fsanitize=address
      )
    endfunction()
  elseif(ENV_MSVC)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding sanitization to \"${_name}\".)

      target_compile_options(
        ${_mod}
        PRIVATE
        /Zi
        # /fsanitize=address
      )
    endfunction()
  elseif(ENV_GNU)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding sanitization to \"${_name}\".)

      target_compile_options(
        ${_mod}
        PRIVATE
        -Og
        -ggdb
        # -fsanitize=address,leak,undefined
      )
    endfunction()
  elseif(ENV_CLANG)
    function(env_target_optimize _name)
      env_prefix_with_project_name(${_name} _mod)
      env_log(Adding sanitization to \"${_name}\".)

      target_compile_options(
        ${_mod}
        PRIVATE
        -glldb
        # -fsanitize=address,undefined
      )
    endfunction()
  else()
    function(env_target_optimize _name)
    endfunction()
  endif()
endif()


# Nonconforming optimization --------------------------------------------------

if(ENV_CLANG_CL)
  function(env_target_optimize_nonconforming _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding nonconforming optimizations to \"${_name}\".)

    target_compile_options(${_mod} PRIVATE /GR-)
  endfunction()
elseif(ENV_MSVC)
  function(env_target_optimize_nonconforming _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding nonconforming optimizations to \"${_name}\".)

    target_compile_options(${_mod} PRIVATE /GR-)
  endfunction()
elseif(ENV_GNU)
  function(env_target_optimize_nonconforming _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding nonconforming optimizations to \"${_name}\".)

    target_compile_options(${_mod} PRIVATE -fno-rtti)
  endfunction()
elseif(ENV_CLANG)
  function(env_target_optimize_nonconforming _name)
    env_prefix_with_project_name(${_name} _mod)
    env_log(Adding nonconforming optimizations to \"${_name}\".)

    target_compile_options(${_mod} PRIVATE -fno-rtti)
  endfunction()
else()
  function(env_target_optimize_nonconforming _name)
  endfunction()
endif()


# -----------------------------------------------------------------------------
# Targets
# -----------------------------------------------------------------------------

# Atomic targets --------------------------------------------------------------

function(env_project_pch)
  if(EXISTS "${PROJECT_SOURCE_DIR}/include/${LOWER_PROJECT_NAME}/pch.hpp")
    env_use_lower_project_name()
    env_prefix(pch ${LOWER_PROJECT_NAME} _mod)
    env_log(" - Adding precompiled headers of \"${PROJECT_NAME}\". - ")

    file(
      GLOB_RECURSE
      _sources
      CONFIGURE_DEPENDS
      "${PROJECT_SOURCE_DIR}/pch/*.cpp"
    )
    if(_sources)
      add_library(${_mod} STATIC EXCLUDE_FROM_ALL ${_sources})

      env_target_link(${_mod} PUBLIC ${ARGN})
      env_target_include(${_mod} PUBLIC "${PROJECT_SOURCE_DIR}/include")
      env_target_precompile(${_mod} PUBLIC <${LOWER_PROJECT_NAME}/pch.hpp>)

      env_target_conform(${_mod})
      env_target_suppress(${_mod})
      env_target_optimize(${_mod})

      env_target_set_pie(${_mod})
      env_set_cpp17(${_mod})

    else()
      add_library(${_mod} INTERFACE EXCLUDE_FROM_ALL)

      env_target_link(${_mod} INTERFACE ${ARGN})
      env_target_include(${_mod} INTERFACE "${PROJECT_SOURCE_DIR}/include")
      env_target_precompile(${_mod} INTERFACE <${LOWER_PROJECT_NAME}/pch.hpp>)

    endif()

    add_library(${LOWER_PROJECT_NAME}::pch ALIAS ${_mod})

  else()

    add_library(${_mod} INTERFACE EXCLUDE_FROM_ALL)
  endif()
endfunction()


function(env_add_library _name)
  env_use_lower_project_name()
  env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
  env_log("Adding library \"${_name}\".")

  add_library(${_mod} ${ARGN})

  env_target_conform(${_mod})
  env_target_warn(${_mod})
  env_target_optimize(${_mod})

  env_target_set_pie(${_mod})
  env_set_cpp17(${_mod})
endfunction()

function(env_add_suppressed _name)
  env_use_lower_project_name()
  env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
  env_log("Adding suppressed \"${_name}\".")

  add_library(${_mod} ${ARGN})

  env_target_conform(${_mod})
  env_target_suppress(${_mod})
  env_target_optimize(${_mod})

  env_target_set_pie(${_mod})
  env_set_cpp17(${_mod})
endfunction()


function(env_add_executable _name)
  env_use_lower_project_name()
  env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
  env_log("Adding executable \"${_name}\".")

  add_executable(${_mod} ${ARGN})

  env_target_conform(${_mod})
  env_target_warn(${_mod})
  env_target_optimize(${_mod})

  env_set_cpp17(${_mod})
endfunction()


function(env_add_interface _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log("Adding interface \"${_name}\".")

  add_library(${_mod} INTERFACE)

  env_target_link(${_mod} INTERFACE ${ARGN})
endfunction()

function(env_add_import _name)
  env_prefix_with_project_name(${_name} _mod)
  env_log("Adding import \"${_name}\".")

  add_library(${_mod} INTERFACE IMPORTED GLOBAL)

  env_target_link(${_mod} INTERFACE ${ARGN})
endfunction()


function(env_add_alias _name)
  env_use_lower_project_name()
  env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
  env_log("Adding alias \"${LOWER_PROJECT_NAME}::${_name}\".")

  add_library(${LOWER_PROJECT_NAME}::${_name} ALIAS ${_mod})
endfunction()


# Compound targets ------------------------------------------------------------

function(env_add_objects _name)
  env_use_upper_project_name()
  env_log(- Adding objects \"${_name}\". -)

  env_add_library(${_name} OBJECT ${ARGN})

  env_target_definitions(
    ${_name}
    PUBLIC
    "${UPPER_PROJECT_NAME}_OBJECT=1"
  )

  env_add_alias(${_name})
endfunction()

function(env_add_static _name)
  env_use_upper_project_name()
  env_log(- Adding static \"${_name}\". -)

  env_add_library(${_name} STATIC ${ARGN})

  env_target_definitions(
    ${_name}
    PUBLIC
    "${UPPER_PROJECT_NAME}_STATIC=1"
  )

  env_add_alias(${_name})
endfunction()

function(env_add_shared _name)
  env_use_upper_project_name()
  env_log(- Adding shared \"${_name}\". -)

  env_add_library(${_name} SHARED ${ARGN})

  env_set_visibility(${_name} HIDDEN)
  env_target_definitions(
    ${_name}
    PUBLIC
    "${UPPER_PROJECT_NAME}_SHARED=1"
    PRIVATE
    "${UPPER_PROJECT_NAME}_EXPORT=1"
    INTERFACE
    "${UPPER_PROJECT_NAME}_IMPORT=1"
  )

  env_add_alias(${_name})
endfunction()


function(env_add_module _name)
  env_use_upper_project_name()
  env_log(- Adding module \"${_name}\". -)

  env_add_library(${_name} MODULE ${ARGN})

  env_set_visibility(${_name} HIDDEN)
  env_target_definitions(
    ${_name}
    PUBLIC
    "${UPPER_PROJECT_NAME}_MODULE=1"
    PRIVATE
    "${UPPER_PROJECT_NAME}_EXPORT=1"
    INTERFACE
    "${UPPER_PROJECT_NAME}_IMPORT=1"
  )

  env_add_alias(${_name})
endfunction()


function(env_add_app _name)
  env_use_upper_project_name()
  env_log(" - Adding app \"${_name}\". - ")

  env_add_executable(${_name} ${ARGN})

  env_target_definitions(
    ${_name}
    PUBLIC
    "${UPPER_PROJECT_NAME}_APP=1"
  )
endfunction()


enable_testing()
include(GoogleTest)

function(env_add_test _name)
  env_use_upper_project_name()
  env_log(" - Adding test \"${_name}\". - ")

  env_add_executable(${_name} ${ARGN})

  env_target_definitions(
    ${_name}
    PUBLIC
    "${UPPER_PROJECT_NAME}_TEST=1"
  )

  env_use_lower_project_name()
  env_prefix(${_name} ${LOWER_PROJECT_NAME} _mod)
  gtest_discover_tests(${_name})
endfunction()

function(env_add_bench _name)
  if(NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    env_use_upper_project_name()
    env_log(" - Adding bench \"${_name}\". - ")

    env_add_executable(${_name} ${ARGN})

    env_target_definitions(
      ${_name}
      PUBLIC
      "${UPPER_PROJECT_NAME}_BENCH=1"
    )
  endif()
endfunction()


function(env_add_dep _name)
  env_log(" - Adding dependency \"${_name}\". - ")

  foreach(_link IN LISTS ARGN)
    get_target_property(_type ${_link} TYPE)
    if(NOT _type STREQUAL INTERFACE_LIBRARY)
      env_target_clear_warn(${_link})
      env_target_suppress(${_link})

      env_target_conform(${_link})
    endif()
  endforeach()

  env_add_import(${_name} ${ARGN})
  env_add_alias(${_name})
endfunction()

function(env_add_export _name)
  env_log(" - Adding export \"${_name}\". - ")

  env_add_interface(${_name} ${ARGN})
  env_add_alias(${_name})
endfunction()


# Custom targets --------------------------------------------------------------

function(env_target_hook _dep)
  cmake_parse_arguments(PARSED "" "" "INTO" ${ARGN})

  foreach(_target IN LISTS PARSED_INTO)
    if(NOT TARGET ${_target})
      env_log(Adding custom \"${_target}\" and hooking \"${_dep}\" into it.)

      add_custom_target(${_target})
      add_dependencies(${_target} ${_dep})
    else()
      get_target_property(_deps ${_target} MANUALLY_ADDED_DEPENDENCIES)

      list(FIND _deps ${_dep} _index)
      if(_index EQUAL -1)
        env_log(Hooking \"${_dep}\" into \"${_target}\".)

        add_dependencies(${_target} ${_dep})
      endif()
    endif()
  endforeach()
endfunction()

function(env_target_reduce _target)
  cmake_parse_arguments(PARSED "" "INTO" "" ${ARGN})

  if(NOT PARSED_INTO)
    env_suffix(${_target} reduced PARSED_INTO)
  endif()

  get_target_property(_deps ${_target} MANUALLY_ADDED_DEPENDENCIES)
  if(NOT _deps STREQUAL _deps-NOTFOUND)
    env_log(- Reducing \"${_target}\" into \"${PARSED_INTO}\". -)
    env_add_app(${PARSED_INTO})
    env_target_hook(${_target} INTO ${PARSED_INTO})

    set(_commands "")
    foreach(_dep IN LISTS _deps)
      get_target_property(_type ${_dep} TYPE)
      if(_type STREQUAL EXECUTABLE)
        env_target_get_native_location(${_dep} _loc)
        env_log(Adding call to \"${_loc}\" in \"${PARSED_INTO}\".)
        set(
          _commands
          "
    std::cout << \"--------------------------------------------\" << std::endl;
    std::cout << R\"(Running executable at: \"${_loc}\")\" << std::endl;
    std::cout << \"--------------------------------------------\" << std::endl;

    std::system(R\"(\"${_loc}\")\");\

${_commands}\
"
        )
      endif()
    endforeach()

    set(
      _source
      "\
#include <cstdlib>
#include <iostream>

int main(int, char**)
{${_commands}

    return EXIT_SUCCESS;
}
"
    )

    set(_run_file "${PROJECT_BINARY_DIR}/.reduced/${PARSED_INTO}.cpp")
    file(WRITE "${_run_file}" "${_source}")
    env_target_sources(${PARSED_INTO} ${_run_file})

    env_target_set_bin_output(${PARSED_INTO})
  endif()
endfunction()


# -----------------------------------------------------------------------------
# Automatic configuration
# -----------------------------------------------------------------------------

# Subdirectory ----------------------------------------------------------------

function(env_subdirectory)
  env_log(Adding subdirectories: \"${ARGN}\".)

  set(_previous_log_level ${CMAKE_MESSAGE_LOG_LEVEL})
  set(_previous_log_indent ${CMAKE_MESSAGE_INDENT})

  set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
  set(CMAKE_MESSAGE_INDENT "${_previous_log_indent}    ")
  add_subdirectory(${ARGN})

  set(CMAKE_MESSAGE_LOG_LEVEL ${_previous_log_level})
  set(CMAKE_MESSAGE_INDENT ${_previous_log_indent})
endfunction()


# Scaffold --------------------------------------------------------------------

set(
  __env_scaffold_default_include_dirs
  "/include"
  CACHE STRING
  "Default include directory for scaffolded libraries."
)

set(
  __env_scaffold_default_src_glob
  "/src/*.cpp;/src/*.cc;/src/*.c;/source/*.cpp;/source/*.cc;/source/*.c"
  CACHE STRING
  "Default list of globbing patterns for scaffolded libraries."
)

set(
  __env_scaffold_default_lib_glob
  "/bin/*.lib;/bin/*.a"
  CACHE STRING
  "Default list of globbing patterns for scaffolded libraries."
)

set(
  __env_scaffold_default_pch_glob
  "/include/env/pch.hpp"
  CACHE STRING
  "Default location of precompiled header for scaffolded libraries."
)


function(env_scaffold _src_dir)
  cmake_parse_arguments(
    PARSED
    ""
    "NAME;BINARY_DIR"
    "OPTIONS;INCLUDE_DIRS;SRC_GLOB;LIB_GLOB;PCH_GLOB"
    ${ARGN}
  )

  env_log(Scaffolding \"${_src_dir}\".)


  if(EXISTS "${_src_dir}/CMakeLists.txt")
    foreach(_option IN LISTS PARSED_OPTIONS)
      separate_arguments(_option UNIX_COMMAND "${_option}")
      set(${_option} CACHE BOOL "" FORCE)

      env_log(Setting \"${_name}\" option \"${_option}\".)
    endforeach()

    if(PARSED_BINARY_DIR)
      env_subdirectory("${_src_dir}" "${PARSED_BINARY_DIR}")
    else()
      env_subdirectory("${_src_dir}")
    endif()


  else()
    if(NOT PARSED_NAME)
      env_log(
        FATAL_ERROR
        Please provide a name for the scaffolded
        library that doesn't have a CMakeLists.txt file.
      )
    endif()

    set(_name ${PARSED_NAME})
    env_prefix_with_project_name(${_name} _prefixed)


    if(NOT PARSED_INCLUDE_DIRS)
      set(PARSED_INCLUDE_DIRS "${__env_scaffold_default_include_dirs}")
    endif()
    if(NOT PARSED_SRC_GLOB)
      set(PARSED_SOURCE_GLOB "${__env_scaffold_default_src_glob}")
    endif()
    if(NOT PARSED_LIB_GLOB)
      set(PARSED_LIB_GLOB "${__env_scaffold_default_lib_glob}")
    endif()
    if(NOT PARSED_PCH_GLOB)
      set(PARSED_PCH_GLOB "${__env_scaffold_default_pch_glob}")
    endif()

    set(_include_dirs "")
    foreach(_include_dir IN LISTS PARSED_INCLUDE_DIRS)
      list(APPEND _include_dirs "${_src_dir}${_include_dir}")
    endforeach()

    set(_sources "")
    foreach(_src_glob IN LISTS PARSED_SRC_GLOB)
      file(GLOB_RECURSE _globbed "${_src_dir}${_src_glob}")
      list(APPEND _sources ${_globbed})
    endforeach()

    set(_libs "")
    foreach(_lib_glob IN LISTS PARSED_LIB_GLOB)
      file(GLOB_RECURSE _globbed "${_src_dir}${_lib_glob}")
      list(APPEND _libs ${_globbed})
    endforeach()

    set(_pch "")
    foreach(__pch IN LISTS PARSED_PCH_GLOB)
      file(GLOB_RECURSE _globbed "${_src_dir}${__pch}")
      list(APPEND _pch ${_globbed})
    endforeach()


    if(_sources)
      env_log(Adding for \"${_name}\" a static library \"${_prefixed}\".)

      add_library(${_prefixed} STATIC IMPORTED GLOBAL)
      env_target_include(${_prefixed} PUBLIC ${_include_dirs})
      env_target_sources(${_prefixed} PRIVATE ${_sources})
      env_target_link(${_prefixed} PUBLIC ${_libs})
      env_target_precompile(${_prefixed} PRIVATE ${_pch})

      env_target_optimize(${_prefixed})
      env_target_suppress(${_prefixed})

      env_target_set_pie(${_prefixed})

    else()
      env_log(Adding for \"${_name}\" an interface library \"${_prefixed}\".)

      add_library(${_prefixed} INTERFACE IMPORTED GLOBAL)
      env_target_link(${_prefixed} INTERFACE ${_libs})
      env_target_include(${_prefixed} INTERFACE ${_include_dirs})
      env_target_precompile(${_prefixed} INTERFACE ${_pch})

    endif()

    env_add_alias(${_name})
  endif()
endfunction()


# Stratify --------------------------------------------------------------------

# TODO: with 3.18 cmake_language


# -----------------------------------------------------------------------------
# Fetch
# -----------------------------------------------------------------------------

set(
  __env_fetch_dir
  "${CMAKE_SOURCE_DIR}/.fetch"
  CACHE STRING
  "Download directory for fetching dependencies."
  FORCE
)

set(
  __env_fetch_build_dir
  "${PROJECT_BINARY_DIR}/.fetch"
  CACHE STRING
  "Build directory for fetched dependencies."
  FORCE
)


include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${__env_fetch_dir})


function(env_fetch _name)
  env_use_lower_project_name()
  cmake_parse_arguments(PARSED "NO_EXTRACT" "INTO" "FROM;SCAFFOLD" ${ARGN})

  env_prefix(${_name} ${LOWER_PROJECT_NAME} _prefixed)
  env_suffix(${_prefixed} fetch _mod)

  if(PARSED_INTO)
    set(_src_dir "${__env_fetch_dir}/${PARSED_INTO}/${_name}")
    set(_bin_dir "${__env_fetch_build_dir}/${PARSED_INTO}/${_name}/bin")
    set(_sub_dir "${__env_fetch_build_dir}/${PARSED_INTO}/${_name}/sub")

    set(
      _populated_file
      "${__env_fetch_dir}/.process/${PARSED_INTO}/${_name}.populated"
    )
    set(
      _lock_file
      "${__env_fetch_dir}/.process/${PARSED_INTO}/${_name}.lock"
    )
  else()
    set(_src_dir "${__env_fetch_dir}/${_name}")
    set(_bin_dir "${__env_fetch_build_dir}/${_name}/bin")
    set(_sub_dir "${__env_fetch_build_dir}/${_name}/sub")

    set(
      _populated_file
      "${__env_fetch_dir}/.process/${_name}.populated"
    )
    set(
      _lock_file
      "${__env_fetch_dir}/.process/${_name}.lock"
    )
  endif()

  set(${_mod}_src_dir "${_src_dir}" PARENT_SCOPE)
  set(${_mod}_bin_dir "${_bin_dir}" PARENT_SCOPE)


  file(LOCK "${_lock_file}")
  if(NOT EXISTS "${_populated_file}")
    env_log("Fetching \"${_name}\" into \"${_src_dir}\".")

    if(PARSED_NO_EXTRACT)
      fetchcontent_populate(
        ${_mod}
        QUIET
        ${PARSED_FROM}
        DOWNLOAD_NO_EXTRACT TRUE
        DOWNLOAD_DIR "${_src_dir}"
        SOURCE_DIR "${_src_dir}"
        BINARY_DIR "${_bin_dir}"
        SUBBUILD_DIR "${_sub_dir}"
      )
    else()
      fetchcontent_populate(
        ${_mod}
        QUIET
        ${PARSED_FROM}
        SOURCE_DIR "${_src_dir}"
        BINARY_DIR "${_bin_dir}"
        SUBBUILD_DIR "${_sub_dir}"
      )
    endif()

    file(WRITE "${_populated_file}" YES)
  else()
    env_log("Already fetched \"${_name}\" into \"${_src_dir}\".")
  endif()
  file(LOCK "${_lock_file}" RELEASE)


  if(PARSED_SCAFFOLD)
    env_scaffold(
      "${_src_dir}"
      NAME "${_name}"
      BINARY_DIR "${_bin_dir}"
      ${PARSED_SCAFFOLD}
    )
  endif()
endfunction()


# -----------------------------------------------------------------------------
# Projects
# -----------------------------------------------------------------------------

# Initialization --------------------------------------------------------------

function(env_project_default_options)
  env_use_upper_project_name()
  env_log(---!!!--- Initializing project \"${PROJECT_NAME}\". ---!!!---)

  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    option(
      ${UPPER_PROJECT_NAME}_COMPILER_MESSAGES
      "Turn on compiler messages for ${PROJECT_NAME}."
      ON
    )

  else()
    option(
      ${UPPER_PROJECT_NAME}_COMPILER_MESSAGES
      "Turn on compiler messages for ${PROJECT_NAME}."
      OFF
    )

  endif()


  option(
    ${UPPER_PROJECT_NAME}_BUILD_STATIC
    "Build ${PROJECT_NAME} static."
    ON
  )

  option(
    ${UPPER_PROJECT_NAME}_BUILD_SHARED
    "Build ${PROJECT_NAME} shared."
    OFF
  )

  option(
    ${UPPER_PROJECT_NAME}_BUILD_OBJECTS
    "Build ${PROJECT_NAME} objects."
    OFF
  )


  option(
    ${UPPER_PROJECT_NAME}_BUILD_APPS
    "Build ${PROJECT_NAME} apps."
    OFF
  )


  option(
    ${UPPER_PROJECT_NAME}_BUILD_BINDS
    "Build ${PROJECT_NAME} bindings."
    OFF
  )

  if(${UPPER_PROJECT_NAME}_BUILD_BINDS)
    set(
      ${UPPER_PROJECT_NAME}_BUILD_BIND_UTILS
      ON CACHE BOOL
      "Build ${PROJECT_NAME} binding utils."
      FORCE
    )
  else()
    option(
      ${UPPER_PROJECT_NAME}_BUILD_BIND_UTILS
      "Build ${PROJECT_NAME} binding utils."
      OFF
    )
  endif()


  option(
    ${UPPER_PROJECT_NAME}_BUILD_TESTS
    "Build ${PROJECT_NAME} tests."
    OFF
  )

  option(
    ${UPPER_PROJECT_NAME}_BUILD_BENCHMARKS
    "Build ${PROJECT_NAME} benchmarks."
    OFF
  )

  option(
    ${UPPER_PROJECT_NAME}_BUILD_CI
    "Build ${PROJECT_NAME} CI."
    OFF
  )


  option(
    ${UPPER_PROJECT_NAME}_BUILD_EXAMPLES
    "Build ${PROJECT_NAME} examples."
    OFF
  )

  option(
    ${UPPER_PROJECT_NAME}_BUILD_DOCS
    "Build ${PROJECT_NAME} docs."
    OFF
  )


  option(
    ${UPPER_PROJECT_NAME}_BUILD_EXTRAS
    "Build ${PROJECT_NAME} extras."
    OFF
  )
endfunction()


# Targets ---------------------------------------------------------------------

function(env_project_objects)
  env_use_upper_project_name()
  if(${UPPER_PROJECT_NAME}_BUILD_OBJECTS AND ARGN)
    env_log(-!- Adding objects for \"${PROJECT_NAME}\". -!-)
    env_add_objects(objects ${ARGN})

    env_use_lower_project_name()
    env_target_link(objects PRIVATE ${LOWER_PROJECT_NAME}::pch)
    env_target_include(objects PUBLIC ${PROJECT_SOURCE_DIR}/include)
  endif()
endfunction()

function(env_project_static)
  env_use_upper_project_name()
  if(${UPPER_PROJECT_NAME}_BUILD_STATIC AND ARGN)
    env_log(-!- Adding static for \"${PROJECT_NAME}\". -!-)
    env_add_static(static ${ARGN})

    env_use_lower_project_name()
    env_target_link(static PRIVATE ${LOWER_PROJECT_NAME}::pch)
    env_target_include(static PUBLIC ${PROJECT_SOURCE_DIR}/include)

    env_target_set_bin_output(${LOWER_PROJECT_NAME}_static)
  endif()
endfunction()

function(env_project_shared)
  env_use_upper_project_name()
  if(${UPPER_PROJECT_NAME}_BUILD_SHARED AND ARGN)
    env_log(-!- Adding shared for \"${PROJECT_NAME}\". -!-)
    env_add_shared(shared ${ARGN})

    env_use_lower_project_name()
    env_target_link(shared PRIVATE ${LOWER_PROJECT_NAME}::pch)
    env_target_include(shared PUBLIC ${PROJECT_SOURCE_DIR}/include)

    env_target_set_bin_output(${LOWER_PROJECT_NAME}_shared)
  endif()
endfunction()

function(env_project_export)
  cmake_parse_arguments(PARSED "SHARE_PCH" "" "SOURCES" ${ARGN})

  if(PARSED_SOURCES)
    env_log(-!- Adding export for \"${PROJECT_NAME}\". -!-)
    env_add_export(export)

    env_use_upper_project_name()
    env_use_lower_project_name()
    if(${UPPER_PROJECT_NAME}_BUILD_STATIC)
      env_add_suppressed(
        suppressed STATIC EXCLUDE_FROM_ALL
        ${PARSED_SOURCES}
      )
      env_target_link(
        export
        INTERFACE
        ${LOWER_PROJECT_NAME}_suppressed
      )

    elseif(${UPPER_PROJECT_NAME}_BUILD_OBJECTS)
      env_add_suppressed(
        suppressed OBJECT EXCLUDE_FROM_ALL
        ${PARSED_SOURCES}
      )
      env_target_sources(
        export
        INTERFACE
        $<TARGET_OBJECTS:${LOWER_PROJECT_NAME}_suppressed>
      )

    elseif(${UPPER_PROJECT_NAME}_BUILD_SHARED)
      env_add_suppressed(
        suppressed SHARED EXCLUDE_FROM_ALL
        ${PARSED_SOURCES}
      )
      env_target_link(
        export
        INTERFACE
        ${LOWER_PROJECT_NAME}_suppressed
      )

    else()
      env_log(
        WARNING
        Failed to create an export target for \"${PROJECT_NAME}\".
        Please specify a flag to build either
        an objects or static or shared library
        that could be used to create an export target.
      )
    endif()

    if(TARGET ${LOWER_PROJECT_NAME}_suppressed)
      env_add_alias(suppressed)

      if(PARSED_SHARE_PCH)
        env_target_link(suppressed PUBLIC ${LOWER_PROJECT_NAME}::pch)
      else()
        env_target_link(suppressed PRIVATE ${LOWER_PROJECT_NAME}::pch)
      endif()

      env_target_include(suppressed PUBLIC ${PROJECT_SOURCE_DIR}/include)

      env_target_set_bin_output(${LOWER_PROJECT_NAME}_suppressed)
    endif()
  endif()
endfunction()


function(env_project_apps)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_APPS AND
    EXISTS "${PROJECT_SOURCE_DIR}/app"
  )
    env_log(-!- Adding apps for \"${PROJECT_NAME}\". -!-)

    file(
      GLOB_RECURSE
      _apps
      CONFIGURE_DEPENDS
      "${PROJECT_SOURCE_DIR}/app/*.cpp"
    )

    env_use_lower_project_name()
    foreach(_app IN LISTS _apps)
      env_target_name_for(${_app} _target)

      env_add_app(${_target} ${_app} ${ARGN})

      env_target_link(${_target} PRIVATE ${LOWER_PROJECT_NAME}::pch)
      env_target_include(${_target} PRIVATE ${PROJECT_SOURCE_DIR}/include)

      env_target_set_bin_output(
        ${_target}
        SUB "${LOWER_PROJECT_NAME}/apps"
      )
    endforeach()
  endif()
endfunction()


function(env_project_binding_utilities)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_BIND_UTILS AND
    EXISTS "${PROJECT_SOURCE_DIR}/bind/utils"
  )

    env_log(-!- Adding binding utilities for \"${PROJECT_NAME}\". -!-)

    file(
      GLOB
      _bind_utils
      CONFIGURE_DEPENDS
      "${PROJECT_SOURCE_DIR}/bind/utils/*/CMakeLists.txt"
      LIST_DIRECTORIES FALSE
    )

    foreach(_bind_util IN LISTS _bind_utils)
      env_subdirectory("${_bind_util}/..")
    endforeach()
  endif()
endfunction()

function(env_project_bindings)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_BINDS AND
    EXISTS "${PROJECT_SOURCE_DIR}/bind"
  )

    env_log(-!- Adding bindings for \"${PROJECT_NAME}\". -!-)

    file(
      GLOB
      _bindings
      CONFIGURE_DEPENDS
      "${PROJECT_SOURCE_DIR}/bind/*/CMakeLists.txt"
      LIST_DIRECTORIES FALSE
    )

    foreach(_binding IN LISTS _bindings)
      env_subdirectory("${_binding}/..")
    endforeach()
  endif()
endfunction()


function(env_project_tests)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_TESTS AND
    EXISTS "${PROJECT_SOURCE_DIR}/test"
  )

    env_log(-!- Adding tests for \"${PROJECT_NAME}\". -!-)

    file(
      GLOB_RECURSE
      _tests
      CONFIGURE_DEPENDS
      "${PROJECT_SOURCE_DIR}/test/*.cpp"
    )

    env_use_lower_project_name()
    foreach(_test IN LISTS _tests)
      env_target_name_for(${_test} _target)
      env_add_test(${_target} ${_test})

      env_target_link(${_target} PRIVATE ${ARGN})

      env_target_link(${_target} PRIVATE ${LOWER_PROJECT_NAME}::pch)
      env_target_include(${_target} PRIVATE ${PROJECT_SOURCE_DIR}/include)

      env_target_set_bin_output(
        ${_target}
        SUB "test/${LOWER_PROJECT_NAME}"
      )

      env_target_hook(${_target} INTO ${LOWER_PROJECT_NAME}_tests)
    endforeach()
  endif()
endfunction()

function(env_project_benchmarks)
  env_use_upper_project_name()
  if(
    NOT CMAKE_BUILD_TYPE STREQUAL Debug AND
    ${UPPER_PROJECT_NAME}_BUILD_BENCHMARKS AND
    EXISTS "${PROJECT_SOURCE_DIR}/bench"
  )

    env_log(-!- Adding benchmarks for \"${PROJECT_NAME}\". -!-)

    file(
      GLOB_RECURSE
      _benchmarks
      CONFIGURE_DEPENDS
      "${PROJECT_SOURCE_DIR}/bench/*.cpp"
    )

    env_use_lower_project_name()
    foreach(_benchmark IN LISTS _benchmarks)
      env_target_name_for(${_benchmark} _target)
      env_add_bench(${_target} ${_benchmark})

      env_target_link(${_target} PRIVATE ${ARGN})

      env_target_link(${_target} PRIVATE ${LOWER_PROJECT_NAME}::pch)
      env_target_include(${_target} PRIVATE ${PROJECT_SOURCE_DIR}/include)

      env_target_set_bin_output(
        ${_target}
        SUB "bench/${LOWER_PROJECT_NAME}"
      )

      env_target_hook(${_target} INTO ${LOWER_PROJECT_NAME}_benchmarks)
    endforeach()
  endif()
endfunction()

# TODO: configure CI
function(env_project_ci)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_CI AND
    EXISTS "${PROJECT_SOURCE_DIR}/ci/CMakeLists.txt"
  )

    env_log(-!- Adding CI for ${PROJECT_NAME}. -!-)
    env_subdirectory("${PROJECT_SOURCE_DIR}/ci")
  endif()
endfunction()


function(env_project_examples)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_EXAMPLES AND
    EXISTS "${PROJECT_SOURCE_DIR}/example"
  )

    env_log(-!- Adding examples for ${PROJECT_NAME}. -!-)

    file(GLOB _examples "${PROJECT_SOURCE_DIR}/example/*/CMakeLists.txt")

    foreach(_example IN LISTS _examples)
      env_subdirectory("${_example}/..")
    endforeach()
  endif()
endfunction()

# TODO: configure Doxygen
function(env_project_docs)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_DOCS AND
    EXISTS "${PROJECT_SOURCE_DIR}/docs/CMakeLists.txt"
  )

    env_log(-!- Adding docs for \"${PROJECT_NAME}\". -!-)
    env_subdirectory("${PROJECT_SOURCE_DIR}/docs")
  endif()
endfunction()


function(env_project_extras)
  env_use_upper_project_name()
  if(
    ${UPPER_PROJECT_NAME}_BUILD_EXTRAS AND
    EXISTS "${PROJECT_SOURCE_DIR}/extra"
  )

    env_log(-!- Adding extras for ${PROJECT_NAME}. -!-)

    file(GLOB _extras "${PROJECT_SOURCE_DIR}/extra/*/CMakeLists.txt")

    foreach(_extra IN LISTS _extras)
      env_subdirectory("${_extra}/..")
    endforeach()
  endif()
endfunction()


function(env_project_targets)
  env_use_lower_project_name()
  cmake_parse_arguments(
    PARSED
    "SHARE_PCH"
    "SOURCES"
    "DEPS;TEST_DEPS;BENCH_DEPS"
    ${ARGN}
  )

  env_log(---!!!--- Adding project targets for \"${PROJECT_NAME}\". ---!!!---)


  env_project_pch(${PARSED_DEPS})

  file(
    GLOB_RECURSE
    _sources
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
  )

  if(PARSED_SOURCES)
    set(${PARSED_SOURCES} ${_sources})
  endif()


  env_project_objects(${_sources})
  env_project_static(${_sources})
  env_project_shared(${_sources})

  if(PARSED_SHARE_PCH)
    env_project_export(SOURCES ${_sources} SHARE_PCH)
  else()
    env_project_export(SOURCES ${_sources})
  endif()

  env_project_apps(${_sources})

  env_project_binding_utilities()
  env_project_bindings()


  env_project_tests(${PARSED_TEST_DEPS})
  if(TARGET ${LOWER_PROJECT_NAME}_tests)
    env_target_reduce(
      ${LOWER_PROJECT_NAME}_tests
      INTO
      ${LOWER_PROJECT_NAME}_run_tests
    )
  endif()

  env_project_benchmarks(${PARSED_BENCH_DEPS})
  if(TARGET ${LOWER_PROJECT_NAME}_benchmarks)
    env_target_reduce(
      ${LOWER_PROJECT_NAME}_benchmarks
      INTO
      ${LOWER_PROJECT_NAME}_run_benchmarks
    )
  endif()

  env_project_ci()


  env_project_examples()
  env_project_docs()


  env_project_extras()
endfunction()


# Installation ----------------------------------------------------------------

# TODO: install

