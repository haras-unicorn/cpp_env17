if(ENV_CLANG AND CMAKE_BUILD_TYPE STREQUAL Release)
  env_log(Adding extra clang test.)

  add_executable(env_extra_clang_test "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

  target_include_directories(env_extra_clang_test
                             PRIVATE "${PROJECT_SOURCE_DIR}/include")

  target_precompile_headers(env_extra_clang_test PRIVATE
                            "${PROJECT_SOURCE_DIR}/include/env/pch.hpp")

  target_compile_options(env_extra_clang_test PRIVATE -Wall -Wextra -Wpedantic
                                                      -Werror -O3)

  target_compile_features(env_extra_clang_test PRIVATE cxx_std_17)

  set_target_properties(env_extra_clang_test PROPERTIES CXX_EXTENSIONS OFF)
endif()
